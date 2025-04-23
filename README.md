# Static-Tools-for-Detecting-Tri-Level-Programming-Models-MPI-OpenMP-CUDA(MOC)

**Static analysis components for Tri-level programming model using MPI, OpenMP, and CUDA (MOC)**

---

This repository contains the **static components** of a tool designed to assist in the **detection and analysis of hybrid parallel programming models** that integrate **MPI**, **OpenMP**, and **CUDA**. The tool supports static inspection of source code to aid in debugging, bottleneck identification, and correctness verification in high-performance computing (HPC) environments.

---

## Overview

Hybrid programming is increasingly used in modern HPC applications to maximize performance across multi-core CPUs and GPUs. However, debugging and verifying programs that combine **MPI (message passing)**, **OpenMP (shared-memory threading)**, and **CUDA (GPU acceleration)** presents significant challenges.

---

## Development Status

This repository includes only the **first stage** of the tool — the static inspection some components are done and the another currently under active **development**.  
The **second stage**, which will integrate **dynamic analysis techniques**, including runtime error detection, memory access tracking, is **currently under active development**. This dynamic extension will be released in a future update after validation and benchmarking in HPC environments.

---

## Reference

This static tool is introduced and discussed in detail in the following publication:

> Saeed M. Altalhi, "An Architecture for a Tri-Programming Model-Based Parallel Hybrid Testing Tool," *Applied Sciences*, MDPI, vol. 13, no. 21, 11960, 2023.  
> [DOI: 10.3390/app132111960](https://doi.org/10.3390/app132111960)

---

## License

This repository is released under the MIT License for open-access research and development.

---

## Citation

If you use or reference this tool, please cite the following publication:

> Saeed M. Altalhi, “An Architecture for a Tri-Programming Model-Based Parallel Hybrid Testing Tool,” *Applied Sciences*, MDPI, vol. 13, no. 21, 11960, 2023.  
> [DOI: 10.3390/app132111960](https://doi.org/10.3390/app132111960)

---

## Contact
- This project is for educational purposes only and not for commercialization, 
For questions, collaboration, or suggestions, or if you need to get access to source code you can please contact:  
**Saeed M. Altalhi**  
[saeed.altalhi@stu.kau.edu.sa](mailto:saeed.altalhi@stu.kau.edu.sa)

