# 🧠 C-Image-Steganography  
> *Hide secrets in plain sight using the magic of bits.*

---

<p align="center">
  <img src="https://img.shields.io/badge/Made%20with-C-blue.svg" alt="Made with C"/>
  <img src="https://img.shields.io/badge/Build-Passing-brightgreen.svg" alt="Build Passing"/>
  <img src="https://img.shields.io/badge/License-MIT-yellow.svg" alt="License MIT"/>
</p>

---

## 🎨 Overview

**C-Image-Steganography** is a creative blend of art and code 🧬 — a project that allows you to **hide secret text data inside BMP images** using the **Least Significant Bit (LSB)** steganography technique.  

It demonstrates **how binary manipulation can protect information**, making it an excellent project for students exploring **image processing, cybersecurity, and embedded systems**.

---

## 💡 Project Description

Steganography is the ancient art of hiding messages within other forms of data.  
This project brings that art into the digital age — embedding a secret `.txt` file within the pixel data of a `.bmp` image.  

By tweaking the **least significant bit (LSB)** of each pixel color (Red, Green, Blue), data can be hidden **without changing the image’s visible appearance** 👀.

**Core concepts demonstrated:**
- 🔢 Bitwise operations  
- 📂 File I/O and binary data handling  
- 🧩 Image data representation in memory  
- 🧠 Data hiding and recovery techniques  

---

## 🎯 Objectives

- Encode a secret text file into a BMP image.  
- Decode the hidden message from a stego image.  
- Maintain the visual quality of the carrier image.  
- Provide a simple, portable command-line interface.  

---

## 🧩 Features

✅ **Magic String Validation** — verifies the presence of hidden data.  
💾 **BMP File Compatibility** — supports 24-bit bitmap images.  
⚙️ **Makefile Support** — automate build and cleanup easily.  
🧱 **Modular Design** — organized into logical encoding/decoding modules.  
🔐 **Secure and Educational** — perfect for students exploring information hiding.  

---

## 🧑‍💻 Example Use

### 🧠 Encoding Example  
Hide your `secret.txt` message inside `beautiful.bmp`:
```bash
./stego -e beautiful.bmp secret.txt stego.bmp
```

**Sample Output:**
```
----------you have choosed encoding----------
enter the magic string
#*MySecretKey*#
>beautiful.bmp file is present
>secret.txt file is present
>stego.bmp file is created
magic string is encoded successfully
secret file data encoded successfully
<<< Encoding completed successfully >>>
encoding successful
```

---

### 🔓 Decoding Example  
Retrieve your hidden message from `stego.bmp`:
```bash
./stego -d stego.bmp output.txt
```

**Sample Output:**
```
you have choosed decoding
enter the magic string
#*MySecretKey*#
stego.bmp is present
destination file is created
Magic string matched
Secret file data decoded successfully
Decoding Success!!
```

---

## ⚙️ Build & Run

### 🧰 Compilation
```bash
gcc -Wall -g -o stego test_encode.c encode.c decode.c
```

or use the included **Makefile** for simplicity:
```bash
make        # Build
make encode # Run encoding example
make decode # Run decoding example
make clean  # Clean build files
```

---

## 📁 Project Structure
```
📦 C-Steganography
├── beautiful.bmp         # Original BMP image
├── secret.txt            # Secret message file
├── stego.bmp             # Encoded output image
├── encode.c / encode.h   # Encoding logic
├── decode.c / decode.h   # Decoding logic
├── common.h              # Shared constants (like MAGIC_STRING)
├── types.h               # Custom type definitions
├── test_encode.c         # Main driver program
└── Makefile              # Build automation script
```

---

## 🧠 Concept in a Nutshell

Every pixel in a 24-bit BMP image has three color bytes (R, G, B).  
Each color’s least significant bit (LSB) can be altered without visibly changing the image.  
This allows us to **embed message bits within pixel bytes**, creating a *stego-image* that looks identical to the original — but secretly holds data!

---

## 🧑‍🎓 Author

**👤 Nandan U**  
🎓 B.E. Electronics and Communication Engineering  
🏫 Vidya Vikas Institute of Engineering and Technology, Mysuru
💼 Trainee – Embedded Systems, Emertxe Information Technologies, Bengaluru 
📧 [nandanunandanu1@gmail.com](mailto:nandanunandanu1@gmail.com)  

---

## 📜 License
This project is licensed under the **MIT License** — free to use, modify, and share.

---

## 🌟 Quote
> “Thinking should become your capital asset, no matter whatever ups and downs you come across in your life.”  
> — *Dr. A.P.J. Abdul Kalam*

---
