#include "mirtk/Common.h"
#include "mirtk/Options.h"
#include "mirtk/IOConfig.h"
#include "mirtk/GenericImage.h"

using namespace mirtk;

/*
*	Function for extracting slices in a certain direction 
*/
void GetSlice(GreyImage* img1, GreyImage& img_crop, int in_x, int in_y, int in_z)
{
	int maxX, maxY, maxZ;
	maxX = img1->GetX(); 
	maxY = img1->GetY(); 
	maxZ = img1->GetZ(); 
	int n=0;

	if (in_x > 0 || in_y > 0 || in_z > 0) 
	{
		if (in_x > 0)
		{
			img_crop = img1->GetRegion(in_x, 0,0, in_x+1, maxY, maxZ); 
		}
		else if (in_y > 0)
		{
			img_crop = img1->GetRegion(0, in_y,0, maxX, in_y+1, maxZ); 
		}
		else if (in_z > 0)
		{
			img_crop = img1->GetRegion(0, 0,in_z, maxX, maxY, in_z+1);
		}
	}
}

/*
*	The function that does the actual ROC matrix output 		
*/
void GetROC(GreyImage* test_img, GreyImage* gt_img, GreyImage* region_img, Array<int>& target, Array<int>& output)
{
	int maxX, maxY, maxZ; 
	
	
	maxX = test_img->GetX(); 
	maxY = test_img->GetY(); 
	maxZ = test_img->GetZ(); 

	

	for (int x=0;x<maxX;x++) 
	{
		for (int y=0;y<maxY;y++)
		{
			for (int z=0;z<maxZ;z++) 
			{
				int insert_gt = 0, insert_test=0;
				if (region_img && region_img->GetX() > 0)
				{
					if (region_img->Get(x,y,z) > 0)
					{
						// only within the constrained region
						if (gt_img->Get(x,y,z) > 0) insert_gt = 1; 
						if (test_img->Get(x,y,z) > 0) insert_test = 1;
					
						target.push_back(insert_gt);
						output.push_back(insert_test);
					}
					
				}
				else {

					// no constraint region, check everywhere
					if (gt_img->Get(x,y,z) > 0) insert_gt = 1; 
					if (test_img->Get(x,y,z) > 0) insert_test = 1;
					
					target.push_back(insert_gt);
					output.push_back(insert_test);
				}
			
			}
		}
	}

	
}

/*
*	The main driver and functions are called to do specific tasks: 
*	- Slices the images, 
*	- invokes GetROC() on each ROC
*	- Writes the ROC S-by-Q matrix (see MATLAB roc documentation) for each slice 
*/
void ComputeROC(GreyImage* test_img, GreyImage* gt_img, GreyImage* rg_img, int x_y_z, ofstream& file_target, ofstream& file_output)
{
	
	GreyImage test_crop, gt_crop, rg_crop; 
	Array<int> target, output; 

	int x_on=0, y_on=0, z_on=0, maxS;

	double sens, spec;

	switch (x_y_z)
	{
		case 1:
			x_on = 1; 
			maxS = test_img->GetX(); 
			break; 
		case 2:
			y_on = 1; 
			maxS = test_img->GetY(); 
			break; 
		case 3: 
			z_on = 1; 
			maxS = test_img->GetZ(); 
			break;
	}

	for (int s=0;s<maxS;s++)
	{
		GetSlice(test_img, test_crop, s*x_on, s*y_on, s*z_on);		// Slice in x/y/z direction from test image 
		GetSlice(gt_img, gt_crop, s*x_on, s*y_on, s*z_on);		// Slice in x/y/z direction from ground truth image 

		if (rg_img)
		{
			GetSlice(rg_img, rg_crop, s*x_on, s*y_on, s*z_on);		// Slice in x/y/z direction from constraint image 
		}
		
		
		GetROC(&test_crop, &gt_crop, &rg_crop, target, output); 
		
		
		
	}

	// write to file the target and output for ROC MATLAB plotting 

	Array<int> label1, label2;

	for (int i=0;i<target.size();i++)
	{
		if (target[i] > 0) {
			label1.push_back(1); 
			label2.push_back(0);
		}
		else {
			label1.push_back(0); 
			label2.push_back(1);
		}
	}

	// Ready to write to target file
	// We first write the first row 
	for (int i=0;i<target.size();i++)
	{
		file_target << label1[i] << "\t"; 
	}
	file_target << endl;
	// target's second row 
	for (int i=0;i<target.size();i++)
	{
		file_target << label2[i] << "\t"; 
	}


	label1.clear(); 
	label2.clear();

	for (int i=0;i<output.size();i++)
	{
		if (output[i] > 0) {
			label1.push_back(1); 
			label2.push_back(0);
		}
		else {
			label1.push_back(0); 
			label2.push_back(1);
		}
	}

	// Ready to write to output file
	// First we write the first row 
	for (int i=0;i<target.size();i++)
	{
		file_output << label1[i] << "\t"; 
	}
	file_output << endl;
	// target's second row 
	for (int i=0;i<target.size();i++)
	{
		file_output << label2[i] << "\t"; 
	}


}

int main(int argc, char **argv)
{
	int optind;
	bool foundArgs1=false, foundArgs2=false, foundArgs3=false, foundArgs4=false;		// Flags for required options
			
	GreyImage test_img, gt_img, region_img; 
	char* input_f1="", *input_f2="", *input_f3="", *output_txt_1="", *output_txt_2="";
	
	int x_y_z = 3;

	if (argc >= 1) 
	{
		int optind=1;
		
		while ((optind < argc) && (argv[optind][0]=='-')) {
			
			string sw = argv[optind];
			
			if (sw == "-t") {			// required
				optind++;
				input_f1 = argv[optind]; 
				foundArgs1 = true; 
			}
			
			else if (sw == "-g") {		// required
				optind++;
				input_f2 = argv[optind]; 
				foundArgs2 = true; 
			}

			else if (sw == "-r") {		
				optind++;
				input_f3 = argv[optind]; 
			}

			else if (sw == "-o1") {		// required
				optind++;
				output_txt_1 = argv[optind]; 
				foundArgs3 = true; 
			}

			else if (sw == "-o2") {		// required
				optind++;
				output_txt_2 = argv[optind]; 
				foundArgs4 = true; 
			}

					
			else if (sw == "--x") 
			{
				x_y_z = 1;
				cout << "\n\nNote: Along x"; 
			}

			else if (sw == "--y") 
			{
				x_y_z = 2;
				cout << "\n\nNote: Along y"; 
			}

			else if (sw == "--z") 
			{
				x_y_z = 3;
				cout << "\n\nNote: Along z"; 
			}
			
			optind++; 
		}
	}
	
	if (!(foundArgs1 && foundArgs2 && foundArgs3 && foundArgs4))
	{
		cout << "\nUsage:\n\tmirtk generate-roc-data \n\n\t-t <test image> \n\t-g <ground truth>\n\n(Optional)\n=========\n\n\r-r <region image>\n\r-o1 <output for ROC target S-by-Q matrix>\n\t-o2 <output for ROC output S-by-Q matrix>\n\r--x, --y, --z for slicing direction" << endl; 
		exit(0); 
	}

	else
	{
		InitializeIOLibrary();
		//GreyImage img1(input_f1);
		//GreyImage img2(input_f2); 

		UniquePtr<BaseImage> test_img(BaseImage::New(input_f1));
		UniquePtr<BaseImage> gt_img(BaseImage::New(input_f2));

		ofstream fileIO_target, fileIO_output; 
		
		fileIO_target.open(output_txt_1);
		fileIO_output.open(output_txt_2);

		if (strcmp(input_f3, "") != 0)
		{
			UniquePtr<BaseImage> region_img(BaseImage::New(input_f3));
			ComputeROC(new GenericImage<short>(*test_img), new GenericImage<short>(*gt_img), new GenericImage<short>(*region_img), x_y_z, fileIO_target, fileIO_output);
		}
		else 
		{
			ComputeROC(new GenericImage<short>(*test_img), new GenericImage<short>(*gt_img), NULL, x_y_z, fileIO_target, fileIO_output);
		}

		fileIO_target.close();
		fileIO_output.close();

		


	}

}
