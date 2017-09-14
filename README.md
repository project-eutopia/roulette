# Roulette

Roulette is a C++ based Monte Carlo dose calculation engine.


## Building source

First you clone the repository and build the `main` executable.

```sh
git clone https://github.com/project-eutopia/roulette.git roulette
cd roulette
git submodule init && git submodule update
mkdir build
cd build
cmake ..
make -j4 main
```

To build and run the tests, run the following (requires GTest)

```sh
make -j4 tests
./tests
```

The program is then run by passing in the settings as either a string or a JSON file name:

```sh
${ROULETTE_BUILD_DIR}/main ${SETTINGS}
```


## Executable settings

There are two modes to run the program in:  `DoseCalculation` mode and `ParticleSimulation` mode.  The first being used for storing dose deposited by particles, and the second for storing histories of particles.  The settings are stored in JSON format.


### `DoseCalculation` mode

The settings takes the form

```js
{
    "type": "DoseCalculation",
    "dose_calculation": DOSE_CALCULATION_SETTINGS
}
```

where `DOSE_CALCULATION_SETTINGS` stores the settings specific to `DoseCalculation` mode.  It is an object with the following fields:

- `description`:  Optional string, not used in code but can be used to add comments to your settings file.
- `output_folder`:  Path for where to store output dose files.
- `seed`:  Optional random integer seed.
- `compound_table`:  Compound table data file.  This repository includes a data file for general use that uses NIST data:  `data/compounds.json`.
- `density_compound_map`:  A mapping from density values to compound types.  Used for defining specific compounds in a density matrix phantom.  It is an array of objects of the form, where the compound name is the compound name in the `compound_table`:
  ```js
  {"low": 0.005, "high": 0.350, "compound": "Lung Tissue (ICRU-44)"}
  ```
- `phantom`: Defines the 3D density matrix that calculations are done in.  This is either an object which defines a uniform density matrix:
   ```js
    "voxel_grid": {
      "nx": 1,
      "ny": 1,
      "nz": 1,
      "bottom_left": [0, -25, -25],
      "top_right": [50, 25, 25]
    },
    "density": 1.06,
    "compound": "Water, Liquid"
   ```
   or is a binary file with data stored as follows:
   - Three 32 bit integers: `nx`, `ny`, `nz`
   - Three 32 bit floating point numbers:  [x, y, z] coordinates of bottom left corner of 3D volume
   - Three 32 bit floating point numbers:  [x, y, z] coordinates of top right corner of 3D volume
   - Three 32 bit integers: `nx`, `ny`, `nz`
   - `nx*ny*nz` 32 bit floating point numbers for each density voxel, stored such that if the x, y, z indexes are given by `i`, `j`, `k`, then the position of density at `(i,j,k)` is `i + nx*j + nx*ny*k` (x increases first, then y, the finally z).
- `dose_storage`:  Specifies how the output dose is saved to file.
    - When `matrix`:  dose is stored as 3D grid:
        - Three 32 bit integers: `nx`, `ny`, `nz`
        - `nx*ny*nz` 32 bit floating point numbers for each dose value (same storage ordering as `phantom`).
    - When `sparse`:
        - Three 32 bit integers: `nx`, `ny`, `nz`
        - Pairs of (`int32_t`, `float`) where the 32 bit integer is the voxel offset index, and the 32 bit floating point number is the dose value.
    - When storing pointwise, of form
      ```js
      {"type": "pointwise", "points": [[0, 1, 2], [5.0, -5.0, 5.0]]}
      ```
      where `points` is the list of points to store dose deposited to.  The output is
        - One 32 bit integer `N`: the number of dose points
        - `N` 32 bit floating point numbers:  the dose to each dose point
- `source_doses`:  An array of `SourceDose` object representations for each source to calculate dose from.  The dose calculated from each source is stored in a separate dose file: `dose_%d.dose`.  Each source is run on a separate thread, so speed can be improved by splitting sources into separate runs, and combining the dose results at the end.  The form of each object is:
    ```js
    {"number_of_particles": 5000, "weight": 1.0, "source": SOURCE_OBJECT}
    ```
  Each `SOURCE_OBJECT` is itself an object, and is used to allow for polymorphic source definitions.  Here are the possible definitions:
    - When `BeamletSource`, it is an object with properties:
        - `type` is `"BeamletSource"`
        - `source` is [x, y, z] coordinates of source point in cm
        - `bottom_left` is [x, y, z] coordinates of bottom left corner of beamlet rectangle in cm
        - `bottom_right` is [x, y, z] coordinates of bottom right corner of beamlet rectangle in cm
        - `top_right` is [x, y, z] coordinates of top right corner of beamlet rectangle in cm
        - `spectrum` is the spectrum settings (see below)
    - When `BeamSource`, it is an object with properties:
        - `type` is `"BeamSource"`
        - `source` is [x, y, z] coordinates of source point in cm
        - `bottom_left` is [x, y, z] coordinates of bottom left corner of beam rectangle in cm
        - `bottom_right` is [x, y, z] coordinates of bottom right corner of beam rectangle in cm
        - `top_right` is [x, y, z] coordinates of top right corner of beam rectangle in cm
        - `nx` and `ny`:  integers representing grid size of beamlets
        - `spectrum` is the spectrum settings (see below)
        - `fluence` is a `ny` by `nx` matrix of fluence values (e.g. for `nx` = 2, `ny` = 3 it could be [[0,1],[1,1],[1,2]])
    - When `DiracDeltaSource`, it is an object with properties:
        - `type` is `"DiracDeltaSource"`
        - `source` is [x, y, z] coordinates of source point in cm
        - `direction` is [dx, dy, dz] vector specifying the direction of particles
        - `spectrum` is the spectrum settings (see below)
    - When `CompositeSource`, it is an object with properties:
        - `type` is `"CompositeSource"`
        - `sub_sources` is an array of objects with properies:
            - `fraction`: The probability of generating a particle from this source (internally normalized so total fraction is 1.0)
            - `source`: Defines a `SOURCE_OBJECT`


### Spectrum object

A typical 6 MeV photon spectrum file is provided at `data/6_mev_spectrum.json`.  There are two formats:
- When probability density function, the object has properties:
    - `type` is `"pdf"`
    - `data` is an array of pairs:  (energy in MeV, pdf height).  Note that pdf heights are normalized internally to have total integral of 1.0.
- When delta function the object has properties:
    - `type` is `"delta"`
    - `value` is the energy in MeV


## Example files

The source code provides some example phantoms and input files.  See the `examples/` directory.  For example, to run a cross-shaped fluence on a water tank phantom, run the following command (assuming `roulette` has been installed at `/opt/roulette`):

```sh
cd /opt/roulette/examples
/opt/roulette/build/main run.json
```

To read the dose matrix in and display it, a simple Python code is:

```python
import numpy
shape = numpy.fromfile("dose_0.dose", dtype=numpy.int32, count=3)
dose = numpy.fromfile("dose_0.dose", dtype=numpy.float32)[3:].reshape(shape)

import matplotlib.pyplot as plt
plt.contourf(dose[:,:,0]); plt.show()
```


## Compounds

Here is a list of available compounds.

- #### Elements
  - Hydrogen
  - Lithium
  - Boron
  - Carbon,Graphite
  - Nitrogen
  - Oxygen
  - Fluorine
  - Sodium
  - Magnesium
  - Aluminum
  - Silicon
  - Phosphorus
  - Sulfur
  - Chlorine
  - Argon
  - Potassium
  - Calcium
  - Titanium
  - Iron
  - Gallium
  - Arsenic
  - Bromine
  - Silver
  - Cadmium
  - Tellurium
  - Iodine
  - Cesium
  - Barium
  - Cerium
  - Gadolinium
  - Mercury
  - Lead

- #### Compounds
  - A-150 Tissue-Equivalent Plastic
  - Adipose Tissue (ICRU-44)
  - Air, Dry (near sea level)
  - Alanine
  - B-100 Bone-Equivalent Plastic
  - Bakelite
  - Blood, Whole (ICRU-44)
  - Bone, Cortical (ICRU-44)
  - Brain, Grey/White Matter (ICRU-44)
  - Breast Tissue (ICRU-44)
  - C-552 Air-equivalent Plastic
  - Cadmium Telluride
  - Calcium Fluoride
  - Calcium Sulfate
  - 15 mmol L-1 Ceric Ammonium Sulfate Solution
  - Cesium Iodide
  - Concrete, Ordinary
  - Concrete, Barite (TYPE BA)
  - Eye Lens (ICRU-44)
  - Ferrous Sulfate Standard Fricke
  - Gadolinium Oxysulfide
  - Gafchromic Sensor
  - Gallium Arsenide
  - Glass, Borosilicate (Pyrex)
  - Glass, Lead
  - Lithium Fluride
  - Lithium Tetraborate
  - Lung Tissue (ICRU-44)
  - Magnesium Tetroborate
  - Mercuric Iodide
  - Muscle, Skeletal (ICRU-44)
  - Ovary (ICRU-44)
  - Photographic Emulsion (Kodak Type AA)
  - Photographic Emulsion  (Standard Nuclear)
  - Plastic Scintillator, Vinyltoluene
  - Polyethylene
  - Polyethylene Terephthalate, (Mylar)
  - Polymethyl Methacrylate
  - Polystyrene
  - Polytetrafluoroethylene, (Teflon)
  - Polyvinyl Chloride
  - Radiochromic Dye Film, Nylon Base
  - Testis (ICRU-44)
  - Tissue, Soft (ICRU-44)
  - Tissue, Soft (ICRU Four-Component)
  - Tissue-Equivalent Gas, Methane Based
  - Tissue-Equivalent Gas, Propane Based
  - Water, Liquid
