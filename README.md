# ROC (Receiver Operator Characteristic) Curve for 3D voxel classification 
This repository contains code that will generate data for plotting ROC curves using [Matlab's ROC functions)(https://uk.mathworks.com/help/nnet/ref/roc.html) 

## Dependencies
Note that this program uses [MIRTK - Medical Image Registration Library](https://github.com/BioMedIA/MIRTK) 

The dependencies are not great on MIRTK It is simply for accessing pixel values in the image. I envisage that the code can be easily ported into [Insight Toolkit](https://github.com/InsightSoftwareConsortium/ITK) and other imaging libraries. 

## Usage 
The usage for the first tool ```evaluate-slice-dice``` if ``make`` installed under mirtk ``/lib/tools/``:
```
./mirtk evaluate-roc-data
     -t <test image as binary mask> 
     -g <ground truth as binary mask> 
     -o1 <target S-by-Q matrix>
     -o2 <output S-by-Q matrix>

(optional parameters)
     switches --x OR --y OR --z to indicate the slice direction 
     -r <image that defines a region within which the ROC computation is constrained>
     
```



## Author 
```
Dr. Rashed Karim 
Department of Biomedical Engineering 
King's College London 
```
