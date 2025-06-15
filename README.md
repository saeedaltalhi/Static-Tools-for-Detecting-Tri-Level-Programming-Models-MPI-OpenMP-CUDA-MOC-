# Static Tools for Detecting Tri-Level Programming Models (MPI + OpenMP + CUDA)

## Static Analysis for Tri-Level Programming with MPI, OpenMP, and CUDA

This repository includes part of a tool developed to help analyze and debug hybrid parallel programs that combine **MPI**, **OpenMP**, and **CUDA** in C++. It focuses specifically on the **static analysis** side of things — going through the code before it runs to find potential problems, check for correctness, and highlight performance issues in high-performance computing (HPC) environments.

---

## Why This Matters

More and more HPC applications rely on a mix of CPU and GPU programming to get the most out of today’s hardware. Combining message-passing (MPI), shared-memory threading (OpenMP), and GPU acceleration (CUDA) can be powerful, but it also makes programs harder to debug. Catching issues before they cause failures is tricky when all three models are involved — and that’s what this tool is trying to help with.

---

## What's Included

This repository version contains work from the **first stage** of development. Some components for static inspection have been done and are included here. A few more are still being finalized and will be added soon.

There’s also a **second stage** on the way — one that adds **dynamic testing**. That part will focus on catching run-time errors, tracking memory behavior, and watching how the program behaves while it’s running. It’s still being tested and benchmarked for HPC environments, so it’ll be released once it’s ready.

> 🔒 **Note on Access:**  
> This tool has been sold. Based on the buyer’s request, the source code is not shared publicly. Access may be given for academic or research purposes, but only with prior permission.

---

## Published Work

Details about this tool, its design, and its testing approach are described in the following publication:

> Saeed M. Altalhi, *"An Architecture for a Tri-Programming Model-Based Parallel Hybrid Testing Tool,"*  
> Applied Sciences, MDPI, vol. 13, no. 21, 11960, 2023.  
> [DOI: 10.3390/app132111960](https://doi.org/10.3390/app132111960)

---

## License

This project is released under the **MIT License**, for use in academic and research settings.

---

## How to Cite

If you reference this tool in your work, please cite the publication below:

> Saeed M. Altalhi, *"An Architecture for a Tri-Programming Model-Based Parallel Hybrid Testing Tool,"*  
> Applied Sciences, MDPI, vol. 13, no. 21, 11960, 2023.  
> [DOI: 10.3390/app132111960](https://doi.org/10.3390/app132111960)

---

## Contact

This project is for **educational and research use only**, not for commercial applications.

If you’re interested in using or learning more about the tool, or if you have questions, you can reach out by email:  
**Saeed M. Altalhi** 
(mailto:saeed.altalhi@stu.kau.edu.sa)

