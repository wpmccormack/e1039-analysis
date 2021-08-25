# Macro to generate signal events

This macro is to generate signal events to be used for the hit embedding.
The DST file (i.e. `DST.root`) outputted by this macro is used next.

## Alternative

You can use another package for this step, such as `SimChainDev`.
In that case you are recommended to
* Confirm that the geometry configuration is identical, and
* Save only necessary data nodes in DSTs to reduce the size of DSTs, via `Fun4AllDstOutputManager::AddNode()`.

## Default Setting of Event Generation

The default setting configured in `Fun4Sim.C` uses
* The J/psi production via `SQPrimaryParticleGen`,
* The standard geometry configuration, and
* The standard event reconstruction (i.e. tracking & vertexing).

You can modify them as you need.
The reconstruction here makes a result _not_ affected by the hit embedding.
It will be compared to another result reconstructed after the hit embedding.

## Usage

The usage is quite similar to `SimChainDev`.
You use two shell scripts to execute the macro on the grid.

Typically you first execute the following command to execute the macro on local for test;
```
./gridsub.sh sig_20210823_01 0 1 100
```

You then execute the following commands to submit grid jobs;
```
source /e906/app/software/script/setup-jobsub-spinquest.sh
./gridsub.sh sig_20210823_01 1 50 5000
```
