# Negate binary mask 
This repository contains code that will negate a mask. It accepts 3D image files in formats such as NifTii, NRRD and GIPL. The only input required is the 3D image. 

## Dependencies
Note that this program uses [MIRTK - Medical Image Registration Library](https://github.com/BioMedIA/MIRTK) 

The dependencies are not great on MIRTK It is simply for accessing pixel values in the image. I envisage that the code can be easily ported into [Insight Toolkit](https://github.com/InsightSoftwareConsortium/ITK) and other imaging libraries. 

## Usage 
The usage for the first tool ```evaluate-slice-dice``` if ``make`` installed under mirtk ``/lib/tools/``:
```
./mirtk evaluate-negate-mask
     -i <input binary image 0/1> 
     -o <output image file containing negated mask>
```



## Author 
```
Dr. Rashed Karim 
Department of Biomedical Engineering 
King's College London 
```
