# Targeted Read Finder

It should be run for each STR region.

# Download

Clone the project with htslib.

```
git clone --recursive git@github.com:gulfemd/get-targeted-reads
```

Make sure that you have [glib@2.46.2](https://developer.gnome.org/glib/) installed.


# Build

`Makefile` is available for multiple targets. To build local dependencies:

```
make libs
```

To build the tool:

```
make trf
```

Note: If you are getting `Segmentation fault` when running the tool, that means you have to increase the stack size before compiling.

```
ulimit -s unlimited
```

# Run

```
./targeted-read-finder --str <STR_FILE_PATH> --bam <BAM_FILE_PATH> --out <OUTPUT_DIR> --insert-size 400 --include-mates
```

`insert-size` and `include-mates` parameters are optional and default to `0` and `false`, respectively.
