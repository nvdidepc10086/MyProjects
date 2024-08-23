requirements:
numpy==1.24.3
urllib3==1.26.16
matplotlib==3.7.1
zlib==1.2.13
scikit-learn==1.2.2
torch==2.1.0

setup instructions: 
1. Extract all the files from the zip file
2. change directory (cd) to the extracted folder, and save any test data into this root folder
3, Create a new virtualenv and nstall packages from requirements:
	virtualenv Student_env
	cd Student_env
	source bin/activate
	cd -
	pip install -r requirements.txt
4. Copy and paste the following code to open the IPYNB:
	jupyter notebook test.ipynb
