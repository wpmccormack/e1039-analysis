# e1039-analysis/AnaCustomFile

A compact program to analyze a data file with a custom data format (=structure).
It should run fine with no modification only on UVA Rivanna.


## Basic Usage

You can execute the following commands to run this program with the default settings, namely;
* Using a modified version of the E1039 core software (at `/project/ptgroup/Jay/new-core/core-inst`),
* Reading an existing data file that is of the Track-QA v1 format 
    (at `/project/ptgroup/spinquest/MC_merge_files/singMup_x2y2z300_750K/merged_trackQA.root`),
* Processing only the first 10 events in the data file,
* Reconstructing tracks with a predefined spectrometer geometry (in `geom.root`), and
* Writing the reco. tracks in `eval.root` with the `SRecEvent` format.

```
git clone https://github.com/E1039-Collaboration/e1039-analysis.git
cd e1039-analysis/AnaCustomFile
source setup.sh
cmake-this
make-this
cd work
root -b AnaTrackQAFile.C
```

You will find no track in the output file, because the number of processed events is too few and the configuration of kTracker is not yet optimized.


### Advanced Usage

* Analyse all events by `root -b 'AnaTrackQAFile.C(0)'`.
* Analyze another file by `root -b 'AnaTrackQAFile.C(10, "/path/to/your/file.root")'`.
* Analyze a hit-matrix data file by `root -b AnaHitMatrixFile.C`.


## Author

Kenichi Naknao <knakano0524@gmail.com>
