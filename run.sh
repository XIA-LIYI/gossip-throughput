#!/bin/sh
#SBATCH --time=30:00
#SBATCH --partition=standard
#SBATCH --nodes=1
#SBATCH --ntasks=1 --cpus-per-task=128
#SBATCH --ntasks-per-node=1
#SBATCH --nodelist=xcnf14
srun ./a.out