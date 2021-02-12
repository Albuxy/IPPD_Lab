#!/bin/bash

#SBATCH --job-name=ex1
#SBATCH --workdir=.
#SBATCH --output=hello_%j.out
#SBATCH --error=hello_%j.err
#SBATCH --cpus-per-task=1
#SBATCH --ntasks=4
#SBATCH --nodes=1
#SBATCH --time=00:00:30

source /shared/profiles.d/easybuild.sh
module load OpenMPI/3.1.4-GCC-8.3.0

make || exit 1      # Exit if make fails
mpirun ./hello
