# QTMAP3D Project Documentation

## [Conventions](./conventions/README.md)


## [Service JSON](./service-json/README.md)


## How to generate `.pdf` file from `.md`?

1. install "xelatex pdf engine".

2. install "pandoc".

3. install "Dubai" font.

4. run below command:

```SHELL
$ pandoc <your-md-file-name>.md -o <output-file-name>.pdf --pdf-engine=xelatex -V mainfont='Dubai'
```
	
5. `.pdf` file generates in inputed command directory!