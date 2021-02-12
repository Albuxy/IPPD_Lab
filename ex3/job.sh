#!/bin/bash

#SBATCH --job-name=ex3
#SBATCH --workdir=.
#SBATCH --output=bcast_%j.out
#SBATCH --error=bcast_%j.err
#SBATCH --cpus-per-task=1
#SBATCH --ntasks=16
#SBATCH --nodes=1
#SBATCH --time=00:00:30

source /shared/profiles.d/easybuild.sh
module load OpenMPI/3.1.4-GCC-8.3.0

make || exit 1      # Exit if make fails
mpirun ./bcast
