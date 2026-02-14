// SPDX-License-Identifier: MIT

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/JSONCompilationDatabase.h>
#include <clang/Tooling/Tooling.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <unistd.h>
#include <vector>

using namespace clang;
using namespace clang::tooling;

// std::vector<std::tuple<std::string, int, int, int, std::string>> records;
std::map<std::pair<std::string, int>,
         std::tuple<int, int, int, std::string, std::string>>
    records;

class OuterBooleanExprVisitor
    : public RecursiveASTVisitor<OuterBooleanExprVisitor> {
public:
  ASTContext *Context; // Store the ASTContext pointer

  OuterBooleanExprVisitor(ASTContext *Context) : Context(Context) {}

  bool VisitIfStmt(IfStmt *ifStmt) {
    extractAndPrintExpression(ifStmt->getCond(), "if");
    return true;
  }

  bool VisitWhileStmt(WhileStmt *whileStmt) {
    extractAndPrintExpression(whileStmt->getCond(), "while");
    return true;
  }

  bool VisitDoStmt(DoStmt *doStmt) {
    extractAndPrintExpression(doStmt->getCond(), "do");
    return true;
  }

  bool VisitForStmt(ForStmt *forStmt) {
    extractAndPrintExpression(forStmt->getCond(), "for");
    return true;
  }

  bool VisitBinaryOperator(BinaryOperator *binaryOp) {
    if (binaryOp->getOpcode() == BO_Assign) {
      extractAndPrintExpression(binaryOp->getRHS(), "assign");
    }
    return true;
  }

  bool VisitConditionalOperator(ConditionalOperator *condOp) {
    extractAndPrintExpression(condOp->getCond(), "conditional");
    return true;
  }

private:
  void extractAndPrintExpression(Expr *expr, std::string decitionType) {
    if (!expr)
      return;

    expr = expr->IgnoreParenImpCasts();
    LangOptions LangOpts;
    LangOpts.CPlusPlus = true;
    PrintingPolicy Policy(LangOpts);

    std::string exprStr;
    llvm::raw_string_ostream s(exprStr);
    expr->printPretty(s, nullptr, Policy);

    int andCount = 0, orCount = 0;
    countLogicalOperators(expr, andCount, orCount);

    SourceManager &SM = Context->getSourceManager();
    ;
    PresumedLoc PL = SM.getPresumedLoc(expr->getBeginLoc());
    std::string fileName = PL.isValid() ? PL.getFilename() : "unknown";
    int lineNo = PL.isValid() ? PL.getLine() : -1;

    int totalCount = andCount + orCount + 1;
    auto key = std::make_pair(fileName, lineNo);
    auto value =
        std::make_tuple(andCount, orCount, totalCount, s.str(), decitionType);
    if (records.find(key) == records.end()) {
      records[key] = value;
    } else {
      auto &oldValue = records[key];
      // Check whether the new totalCound is greater than the old one
      if (std::get<2>(value) > std::get<2>(oldValue)) {
        std::get<0>(oldValue) = andCount;
        std::get<1>(oldValue) = orCount;
        std::get<2>(oldValue) = totalCount;
        std::get<3>(oldValue) = s.str();
        std::get<4>(oldValue) = decitionType;
      }
    }
  }

  void countLogicalOperators(Expr *expr, int &andCount, int &orCount) {
    if (!expr)
      return;

    if (BinaryOperator *binOp = dyn_cast<BinaryOperator>(expr)) {
      if (binOp->getOpcode() == BO_LAnd) {
        ++andCount;
      } else if (binOp->getOpcode() == BO_LOr) {
        ++orCount;
      }

      // Recursively count logical operators in both operands
      countLogicalOperators(binOp->getLHS(), andCount, orCount);
      countLogicalOperators(binOp->getRHS(), andCount, orCount);
    }
  }
};

class BooleanExpressionConsumer : public clang::ASTConsumer {
public:
  virtual void HandleTranslationUnit(clang::ASTContext &Context) {
    OuterBooleanExprVisitor visitor(&Context);
    visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }
};

class BooleanExpressionFrontendAction : public clang::ASTFrontendAction {
public:
  virtual std::unique_ptr<clang::ASTConsumer>
  CreateASTConsumer(clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
    return std::make_unique<BooleanExpressionConsumer>();
  }
};

std::string csvEscape(const std::string &s) {
  if (s.find(',') != std::string::npos || s.find('\"') != std::string::npos) {
    std::string escaped = s;
    size_t pos = 0;
    while ((pos = escaped.find('\"', pos)) != std::string::npos) {
      escaped.replace(pos, 1, "\"\"");
      pos += 2; // Move past the inserted double quotes
    }
    return '\"' + escaped + '\"';
  } else {
    return s;
  }
}

int main(int argc, const char **argv) {
  llvm::cl::OptionCategory MyToolCategory("my-tool options");
  llvm::cl::opt<std::string> OptCompileCommands(
      "p", llvm::cl::desc("Specify path compile_commands.json"),
      llvm::cl::Required, llvm::cl::cat(MyToolCategory));
  llvm::cl::ParseCommandLineOptions(argc, argv);

  // Load compile_commands.json manually
  std::string ErrorMessage;
  auto CompilationDatabase = JSONCompilationDatabase::loadFromFile(
      OptCompileCommands, ErrorMessage,
      clang::tooling::JSONCommandLineSyntax::AutoDetect);

  if (!CompilationDatabase) {
    llvm::errs() << "Error loading compile_commands.json: " << ErrorMessage
                 << "\n";
    return 1;
  }

  // Extract source files from the loaded database
  std::vector<std::string> sources;
  for (const auto &command : CompilationDatabase->getAllCompileCommands()) {
    sources.push_back(command.Filename);
  }

  // Process each source file
  for (const auto &sourcePath : sources) {
    std::cout << sourcePath << std::endl;

    std::vector<std::string> currentSource = {sourcePath};
    ClangTool tool(*CompilationDatabase, currentSource);

    auto frontendAction =
        newFrontendActionFactory<BooleanExpressionFrontendAction>();

    tool.run(frontendAction.get());
  }

  std::ofstream output_file("results-cpp.csv");
  output_file
      << "File,Line,AND,OR,Total Conditions,Decision Code,Decision Type\n";

  // Iterate over the map
  for (const auto &[key, val] : records) {
    output_file << key.first << "," << key.second << "," << std::get<0>(val)
                << "," << std::get<1>(val) << "," << std::get<2>(val) << ","
                << csvEscape(std::get<3>(val)) << "," << std::get<4>(val)
                << "\n";
  }

  return 0;
}