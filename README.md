# **TERMI ART**

Termi Art is a pixel art pad for the terminal. All drawings can be saved to files, which can be viewed and edited later.  

## <u>**VERSION 0.0.2**</u>
Termi Art version 0.0.2 (v0.0.2) was released on December 3, 2020.
**Features added**
* Compressed files

### <u>**DOWNLOADING AND USAGE:**</u>
To download, go into you terminal and navigate to the directory you want to download Termi Art into. Run git clone to download the repository, and navigate into it. Run make, and then run termi_art. Like so:

```
$ cd ~
$ git clone https://github.com/ari-feiglin/Termi-Art
$ cd Termi-Art
$ make
$ ./termi_art
```

Running termi_art without flags and arguments will cause it to give you an error, and a list of arguments you can pass it. Here it is:

```
USAGE: ./termi_art: <flag> [file path]

FLAGS:
-o: Open without editing
    Adding r (-or) opens a non-compressed files (made in v0.0.0)
-e: Open with editing
    Adding r (-er) opens a non-compressed files (made in v0.0.0)
-n: Create new
```

Pixel art created with termi art will be saved to a file that you specify. File extensions do not matter.
