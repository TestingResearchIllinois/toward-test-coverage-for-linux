# SPDX-License-Identifier: MIT

"""
The script to analyze the complexity of decisions in Linux kernel.

Usage:
    python analyze.py <path> [--type <type>]

Output:
    results-<type>.csv: A CSV file containing the results of the analysis.

Note:
    Currently, the script only supports C files. It only analyzes the
    decision with more than one condition.
"""

import argparse
from pathlib import Path
import csv
import numpy as np
from collections import Counter


class Decision:
    def __init__(
        self,
        and_count: int,
        or_count: int,
        condition: str,
        max_depth: int,
        condition_type: str,
    ) -> None:
        self.and_count = and_count
        self.or_count = or_count
        self.counts = (and_count, or_count, and_count + or_count + 1)
        self.condition = condition
        self.max_depth = max_depth
        self.condition_type = condition_type


def count_conditions(decision_str):
    seperator_symbol = "__SEPERATOR__"
    decision_str = decision_str.replace("(", "").replace(")", "")
    decision_str = decision_str.replace("&&", seperator_symbol).replace(
        "||", seperator_symbol
    )
    conditions = decision_str.split(seperator_symbol)

    trimmed_conditions = [
        cond.strip() for cond in conditions
    ]  # Removing any extra spaces
    return Counter(trimmed_conditions)


def get_max_repeated_condition(decision_str):
    condition_counts = count_conditions(decision_str)
    return condition_counts.most_common(1)[0][1]


def extract_ternary_conditions(code: str):
    def is_ternary(code: str):
        # Check if the code first contains a ':' then a ';' in 3 lines.
        first_three_lines = code.split("\n")[:3]
        first_three_lines_code = "".join(first_three_lines)
        comma_idx = first_three_lines_code.find(":")
        semicolon_idx = first_three_lines_code.find(";")
        return (
            comma_idx != -1
            and semicolon_idx != -1
            and comma_idx < semicolon_idx
        )

    conditions = []
    parenthesis_count = 0
    max_depth = 0
    len_code = len(code)

    for idx in range(len_code):
        char = code[idx]

        # Look for '?'
        if char == "?":
            if is_ternary(code[idx:]):
                # It's a probable ternary; now scan backwards for the condition.
                end_idx = idx - 1
                while end_idx >= 0:
                    if code[end_idx] == ")":
                        parenthesis_count += 1
                        max_depth = max(max_depth, parenthesis_count)
                    elif code[end_idx] == "(":
                        parenthesis_count -= 1
                        if parenthesis_count < 0:
                            # We've reached the beginning of the condition.
                            condition_code = code[end_idx + 1 : idx].strip()
                            conditions.append(
                                (condition_code, max_depth, "ternary")
                            )
                            parenthesis_count = 0
                            max_depth = 0
                            break
                    elif (
                        code[end_idx] in [";", ",", "{", "}"]
                        and parenthesis_count == 0
                    ):
                        # We've reached the end of the condition.
                        condition_code = code[end_idx + 1 : idx].strip()
                        conditions.append(
                            (condition_code, max_depth, "ternary")
                        )
                        parenthesis_count = 0
                        max_depth = 0
                        break
                    end_idx -= 1
    return conditions


def extract_conditions(
    code: str, condition_types: list[str] = ["if", "for", "while", "do"]
):
    conditions = []
    in_condition = False
    parenthesis_count = 0
    max_depth = 0
    condition = ""
    current_condition_type = ""

    for idx, char in enumerate(code):
        if char == "(":
            if in_condition:
                parenthesis_count += 1
                condition += char
                max_depth = max(max_depth, parenthesis_count)
        elif char == ")":
            if in_condition:
                condition += char
                parenthesis_count -= 1

                if parenthesis_count == 0:
                    conditions.append(
                        (condition, max_depth, current_condition_type)
                    )
                    condition = ""
                    in_condition = False
                    max_depth = 0
        else:
            if in_condition:
                condition += char

        # Check if we are entering a condition.
        for condition_type in condition_types:
            if (
                code[idx : idx + len(condition_type) + 1]
                == f"{condition_type}("
                or code[idx : idx + len(condition_type) + 2]
                == f"{condition_type} ("
            ) and not in_condition:
                current_condition_type = condition_type
                in_condition = True
                condition += char
                break
    return conditions


def count_file(code: str) -> tuple[Decision]:
    # Find all conditions by searching condition_type
    conditions = extract_conditions(code)
    conditions += extract_ternary_conditions(code)

    decision_list = []
    # Count '&&' and '||' within each
    for condition, max_depth, condition_type in conditions:
        and_count = condition.count("&&")
        or_count = condition.count("||")
        decision_list.append(
            Decision(and_count, or_count, condition, max_depth, condition_type)
        )
    return tuple(decision_list)


def analyze_path(path: Path):
    file_counts = {}
    if path.is_dir():
        # Analyze all files in the directory recursively
        for filename in path.iterdir():
            new_file_counts = analyze_path(filename)
            file_counts.update(new_file_counts)
    elif path.is_file() and (
        path.name.endswith(".c") or path.name.endswith(".h")
    ):
        decision_list = count_file(path.read_text())
        if len(decision_list) > 0:
            file_counts[path] = decision_list
    return file_counts


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "path", type=str, help="Path to the file or directory to analyze"
    )
    args = parser.parse_args()

    path = Path(args.path)
    file_counts = analyze_path(path)

    and_counts = []
    or_counts = []
    total_counts = []
    number_total_decisions = 0
    # Write the results to a CSV file
    with open("results-python.csv", "w", newline="") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(
            [
                "File",
                "AND",
                "OR",
                "Total Conditions",
                "Max Depth",
                "Max Repeated",
                "Decision Code",
                "Decision Type",
            ]
        )
        for filename, decision_list in file_counts.items():
            for d in decision_list:
                # Skip if there is only one condition.
                number_total_decisions += 1
                if d.counts[2] == 1:
                    continue
                and_counts.append(d.counts[0])
                or_counts.append(d.counts[1])
                total_counts.append(d.counts[2])

                writer.writerow(
                    [
                        filename,
                        *d.counts,
                        d.max_depth,
                        get_max_repeated_condition(d.condition),
                        d.condition.replace("\n", ""),
                        d.condition_type,
                    ]
                )

    print("Number of total decisions:", number_total_decisions)
    print("Number of decisions with multiple conditions:", len(total_counts))
    print("Max conditions:", np.max(total_counts))
    print("Max AND operators in decision:", np.max(and_counts))
    print("Max OR operators in decision:", np.max(or_counts))

    print("Average conditions:", np.average(total_counts))
    print("Average AND operators in decision:", np.average(and_counts))
    print("Average OR operators in decision:", np.average(or_counts))

    print("Median number of conditions:", np.median(total_counts))
    print("99% percentile total conditions:", np.percentile(total_counts, 99))
