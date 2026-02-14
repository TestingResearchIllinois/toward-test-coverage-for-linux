<!--
SPDX-License-Identifier: MIT
-->

# Linux Decision Complexity Analysis

This is a collection of scripts and data used to analyze the complexity of decisions in Linux.

## Get Started

Get the source code of Linux kernel from [here](https://github.com/torvalds/linux).

```bash
git clone https://github.com/torvalds/linux --depth=1 $PATH_TO_LINUX_SOURCE
# Analyzed SHA: 99d99825fc075fd24b60cc9cf0fb1e20b9c16b0f
```

## Analysis by Clang

**Step 1:** Compile the Linux kernel with the following command to get `compile_commands.json` file.

```bash
# Install bear
sudo apt install bear

# Compile Linux kernel with clang and generate compile_commands.json
cd $PATH_TO_LINUX_SOURCE
bear -- make CC=clang HOSTCC=clang
cd -
```

**Step 2:** Compile the `analyze.cpp` script with Makefile.

```bash
make all
```

Then, the `analyze` executable will be generated.

**Step 3:** Execute the `analyze` script to analyze the complexity of decisions in Linux, including decision in `if`, `for`, `while`, `do` and boolean assignment, and conditional operator (ternary in C).

```bash
./analyze -p $PATH_TO_LINUX_SOURCE/compile_commands.json
```

The output csv will be `results-cpp.csv` that consists of the following columns:

```csv
File,Line,AND,OR,Total Conditions,Decision Code,Decision Type
```

 - `File` is the path of file in Linux source code repo.
 - `Line` is the line number of the decision.
 - `AND` is the number of AND operators in the decision.
 - `OR` is the number of OR operators in the decision.
 - `Total Conditions` is the total number of conditions in the decision, which is equal to `AND + OR + 1`.
 - `Decision Code` is the code of the decision.
 - `Decision Type` is the type of the decision, which can be `if`, `for`, `while`, `do`, `boolean assignment` and `conditional operator`.

## Analysis by String Matching

Execute the `analyze.py` script to analyze the complexity of decisions in Linux, including decision in `if`, `for`, `while`, `do` and ternary (`decision ? a : b;`).

```bash
python analyze.py $PATH_TO_LINUX_SOURCE 
```

The output of the script is a CSV file `results-python.csv` that consists of the following columns:

```csv
File,AND,OR,Total Conditions,Max Depth,Max Repeated,Decision Code,Decision Type
```

 - `Max Depth` is the maximum depth of parenthesis in the decision.
 - `Max Repeated` is the maximum number of repeated conditions in the decision.
 - `Decision Code` is the code of the decision.


## Data Analysis Results

There are totally 1.4M decisions in Linux. The majority of them are simple decisions (i.e., decisions with only one condition), which is shown in `frequency.png`.

There are some decisions with more than 10 conditions. For example, the `linux/drivers/gpu/drm/amd/display/dc/dml/dcn32/display_mode_vba_32.c` file has a decision with 48 conditions:

```c
if (mode_lib->vba.ScaleRatioAndTapsSupport == true
				&& mode_lib->vba.SourceFormatPixelAndScanSupport == true
				&& mode_lib->vba.ViewportSizeSupport[i][j] == true
				&& !mode_lib->vba.LinkRateDoesNotMatchDPVersion
				&& !mode_lib->vba.LinkRateForMultistreamNotIndicated
				&& !mode_lib->vba.BPPForMultistreamNotIndicated
				&& !mode_lib->vba.MultistreamWithHDMIOreDP
				&& !mode_lib->vba.ExceededMultistreamSlots[i]
				&& !mode_lib->vba.MSOOrODMSplitWithNonDPLink
				&& !mode_lib->vba.NotEnoughLanesForMSO
				&& mode_lib->vba.LinkCapacitySupport[i] == true && !mode_lib->vba.P2IWith420
				//&& !mode_lib->vba.DSCOnlyIfNecessaryWithBPP
				&& !mode_lib->vba.DSC422NativeNotSupported
				&& !mode_lib->vba.MPCCombineMethodIncompatible
				&& mode_lib->vba.ODMCombine2To1SupportCheckOK[i] == true
				&& mode_lib->vba.ODMCombine4To1SupportCheckOK[i] == true
				&& mode_lib->vba.NotEnoughDSCUnits[i] == false
				&& !mode_lib->vba.NotEnoughDSCSlices[i]
				&& !mode_lib->vba.ImmediateFlipOrHostVMAndPStateWithMALLFullFrameOrPhantomPipe
				&& !mode_lib->vba.InvalidCombinationOfMALLUseForPStateAndStaticScreen
				&& mode_lib->vba.DSCCLKRequiredMoreThanSupported[i] == false
				&& mode_lib->vba.PixelsPerLinePerDSCUnitSupport[i]
				&& mode_lib->vba.DTBCLKRequiredMoreThanSupported[i] == false
				&& !mode_lib->vba.InvalidCombinationOfMALLUseForPState
				&& !mode_lib->vba.ImmediateFlipRequiredButTheRequirementForEachSurfaceIsNotSpecified
				&& mode_lib->vba.ROBSupport[i][j] == true
				&& mode_lib->vba.DISPCLK_DPPCLK_Support[i][j] == true
				&& mode_lib->vba.TotalAvailablePipesSupport[i][j] == true
				&& mode_lib->vba.NumberOfOTGSupport == true
				&& mode_lib->vba.NumberOfHDMIFRLSupport == true
				&& mode_lib->vba.EnoughWritebackUnits == true
				&& mode_lib->vba.WritebackLatencySupport == true
				&& mode_lib->vba.WritebackScaleRatioAndTapsSupport == true
				&& mode_lib->vba.CursorSupport == true && mode_lib->vba.PitchSupport == true
				&& mode_lib->vba.ViewportExceedsSurface == false
				&& mode_lib->vba.PrefetchSupported[i][j] == true
				&& mode_lib->vba.VActiveBandwithSupport[i][j] == true
				&& mode_lib->vba.DynamicMetadataSupported[i][j] == true
				&& mode_lib->vba.TotalVerticalActiveBandwidthSupport[i][j] == true
				&& mode_lib->vba.VRatioInPrefetchSupported[i][j] == true
				&& mode_lib->vba.PTEBufferSizeNotExceeded[i][j] == true
				&& mode_lib->vba.DCCMetaBufferSizeNotExceeded[i][j] == true
				&& mode_lib->vba.NonsupportedDSCInputBPC == false
				&& !mode_lib->vba.ExceededMALLSize
				&& (mode_lib->vba.NotEnoughDETSwathFillLatencyHidingPerState[i][j] == false
				|| i == v->soc.num_states - 1)
				&& ((mode_lib->vba.HostVMEnable == false
				&& !mode_lib->vba.ImmediateFlipRequiredFinal)
				|| mode_lib->vba.ImmediateFlipSupportedForState[i][j])
				&& (!mode_lib->vba.DRAMClockChangeRequirementFinal
				|| i == v->soc.num_states - 1
				|| mode_lib->vba.DRAMClockChangeSupport[i][j] != dm_dram_clock_change_unsupported)
				&& (!mode_lib->vba.FCLKChangeRequirementFinal || i == v->soc.num_states - 1
				|| mode_lib->vba.FCLKChangeSupport[i][j] != dm_fclock_change_unsupported)
				&& (!mode_lib->vba.USRRetrainingRequiredFinal
				|| mode_lib->vba.USRRetrainingSupport[i][j])) {
				mode_lib->vba.ModeSupport[i][j] = true;
			} 
```


## Output

```
Number of total decisions: 1637517
Number of decisions with multiple conditions: 187118
Max conditions: 59
Max AND operators in decision: 51
Max OR operators in decision: 31
Average conditions: 2.3245866244829467
Average AND operators in decision: 0.7791821203732404
Average OR operators in decision: 0.5454045041097062
Median number of conditions: 2.0
99% percentile total conditions: 6.0
```