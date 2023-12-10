### dynamic-system

Based on the conducted research, two types of dynamic systems are considered: discrete and continuous. The developed algorithm is aimed at determining and investigating the output characteristics of a complex controlled positive dynamic system in both cases. The result of this work is a software product implemented in the C++ programming language and utilizing the Qt libraries. This product automates the process of obtaining solutions for the tasks of defining and researching output characteristics for both discrete and continuous positive dynamic systems.

### Feature
* C++11
* QT 4.7.3
* QT Creator
* GIT
* Single file for each class

### Installation
#### Clone repository
```base
git clone https://github.com/stas-polos/dynamic-system.git
```

#### Install tools
You need to install the necessary tools described [here](#feature)

#### Run program
To start the program, at Qt Creator press F5

### Program overview
The choice of the C++ programming language and the Qt library version 4.7.3 for developing this program may be influenced by several factors:
* C++11 Support:
    * The C++11 standard introduces many new features and improvements that facilitate development, making the code more readable and secure. Utilizing C++11 functionality can significantly simplify and enhance the program's development, rendering the code more modern and efficient.

* Qt Library:
    * Qt is a powerful library for developing graphical interfaces and cross-platform applications. Version 4.7.3, while not the latest, is stable and might be chosen for its reliability and extensive capabilities.
    * If there are specific features or capabilities in Qt 4.7.3 that are necessary for the project, and there is no need or possibility to migrate to newer versions, this could be a crucial factor in selecting a particular library version.

* Stability:
    * If stability and compatibility with previous versions were crucial during development, choosing a less recent but already proven version might be justified.

Thus, the selection of C++11 and Qt 4.7.3 for writing the program could be motivated by the project's requirements, including functionality, stability, and the existing experience of the development team in using these technologies.

The choice also considered alternatives such as Python and PyQt, but the decision leaned towards developing in the language that the framework itself was written in. While Python and PyQt were viable options, it was deemed more advantageous to align with the language in which the underlying framework was originally implemented. This decision ensures better integration with the core functionalities of the framework, potentially leveraging existing features and optimizations specific to the chosen programming language, in this case, C++.

