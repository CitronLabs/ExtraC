<p align="center">
 <img src="https://github.com/CitronLabs/CitronLabs-SDK/blob/main/__media/Extra-Banner_1x.png">
</p>
<h1> ExtraC: Modernizing C Development </h1>



**ExtraC** is a project dedicated to **modernizing C development**. The objective is to provide a comprehensive set of C libraries and developer tools, simplifying the creation of complex applications in C.

### 📦 ***Packages include:***
- [**Core**](./lib/Core) : The core utilities that every other ExtraC package depends on
- **Others coming soon!**


\**For more information about a particular package please follow the respective link to that package's README file*

## 🔨 Building ExtraC 

Since this project's size is still relatively small, no formal build system has been made for it yet, however as this also means the building from scratch is still very simple, espcially using the prestabished unity build files provided in the source code.

For ease of use and to remain compatibile with future versions of the project we suggest using the included utility scipts **/build/doBuild.sh** for posix systems and **/build/doBuild.ps1** for windows systems. 

This example builds ExtraC's Core package:
#### On Posix:
``` shell
> cd build
> ./doBuild.sh --package core 
```
#### On Windows:
``` shell
> cd build
> ./doBuild.ps1 --package core
```

We **ENSURE** compatibility using this method however functionality may be limited as the project grows and a proper build system is developed.

## Contact

For any inquiries or to follow the project's journey, please feel free to **watch this repository**.

