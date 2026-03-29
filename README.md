# Activitate Structuri de Date (SD) 2026 - Sandu Matei

*[English version below](#data-structures-sd-activity-2026---sandu-matei)*

Acest repository conține codul sursă pentru activitățile și seminarele de la disciplina Structuri de Date (SD), realizate în limbajul C. 

## Conținutul Proiectului

Proiectul este structurat în mai multe fișiere sursă, fiecare abordând concepte diferite ale structurilor de date:

* **`SDD_01.c`**: Lucrul cu structuri de bază (`struct Student`), alocare și dezalocare dinamică a memoriei.
* **`SDD_02.c`**: Utilizarea vectorilor alocați dinamic pentru structuri (`struct Telefon`), funcții de filtrare, căutare și copiere.
* **`SDD_03.c`**: Citirea datelor dintr-un fișier text (`masini.txt`) și stocarea acestora într-un vector alocat dinamic de tip `Masina`.
* **`SDD_04.c`**: Implementarea și manipularea unei liste simplu înlănțuite (adăugare la început/final, calculare preț mediu, ștergere noduri după condiție).
* **`SDD_05.c`**: Implementarea și manipularea unei liste dublu înlănțuite (LDI) (parcurgere în ambele sensuri, adăugare, ștergere după ID, calculare maxime).

## Rulare și Compilare

Repository-ul conține și fișierele de configurare necesare pentru **Microsoft Visual Studio** (`Sandu_Matei_ActivitateSD2026.slnx`, `.vcxproj`). Proiectul poate fi deschis și compilat direct în acest IDE.

> **Notă importantă:** Pentru rularea corectă a programelor din `SDD_03.c`, `SDD_04.c` și `SDD_05.c`, este necesar să creezi un fișier de tip text denumit `masini.txt` în directorul curent de lucru, care să conțină inputul în format CSV.

---

# Data Structures (SD) Activity 2026 - Sandu Matei

This repository contains the source code for the Data Structures (SD) activities and seminars, written in C.

## Project Contents

The project is structured into multiple source files, each covering different data structure concepts:

* **`SDD_01.c`**: Working with basic structures (`struct Student`), dynamic memory allocation, and deallocation.
* **`SDD_02.c`**: Using dynamically allocated arrays for structures (`struct Telefon`), including filtering, searching, and copying functions.
* **`SDD_03.c`**: Reading data from a text file (`masini.txt`) and storing it in a dynamically allocated array of type `Masina`.
* **`SDD_04.c`**: Implementation and manipulation of a singly linked list (adding at the beginning/end, calculating average price, deleting nodes based on conditions).
* **`SDD_05.c`**: Implementation and manipulation of a doubly linked list (DLL) (forward and backward traversal, insertion, deletion by ID, calculating max values).

## Build and Run

The repository includes configuration files for **Microsoft Visual Studio** (`Sandu_Matei_ActivitateSD2026.slnx`, `.vcxproj`). The project can be opened and compiled directly within this IDE.

> **Important Note:** To successfully run the programs in `SDD_03.c`, `SDD_04.c`, and `SDD_05.c`, you must create a text file named `masini.txt` in the current working directory, containing CSV input.
