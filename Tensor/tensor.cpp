// C++ code to perform add, multiply
// and transpose on sparse tensors.
//Also perform add and multiply on dense tensors.
//Can perform multiplication on 1 sparse and 1 dense tensor.
#include <iostream>
using namespace std;


class Tensor
{


public:
	//virtual void sparse_insert(int r, int c, int val) ;
	//virtual void sparse_add(Tensor2D b);
	//virtual void transpose();
	//virtual void sparse_multiply(Tensor2D b);
	//virtual void sparse_print();
	//virtual void dense_add(Tensor2D b);
	//virtual void dense_multiply(Tensor2D b);
	//virtual void dense_insert(int r, int c, int val);
	//virtual void convert_sparse_to_dense();
	//virtual void sparse_dense_multiply(Tensor2D b);
	//virtual void print_dense();
	//virtual void convert_dense_to_sparse();
};




class Tensor2D: public Tensor
{
	//For sparse tensors

	// Maximum number of elements in sparse tensor
	const static int MAX = 100;

	// Double-pointer initialized by
	// the constructor to store
	// the triple-represented form
	int **data;

	// dimensions of tensor
	int row, col;

	// total number of elements in tensor
	int len;

public:
	//Constructor for sparse tensor with 2 dimensions
	Tensor2D(int r, int c)
	{

		// initialize row
		row = r;

		// initialize col
		col = c;

		// initialize length to 0
		len = 0;

		//Array of Pointer to make a tensor
		data = new int *[MAX];

		// Array representation
		// of sparse tensor
		//[,0] represents row
		//[,1] represents col
		//[,2] represents value
		for (int i = 0; i < MAX; i++)
			data[i] = new int[3];
	}

	//Overloaded constructor for dense tensor with 2 dimensions
	Tensor2D(int r, int c, int placeholder)
	{

		// initialize row
		row = r;

		// initialize col
		col = c;

		// initialize length to 0
		len = 0;

		//Array of Pointer to make a tensor
		data = new int *[r];

		// Array representation
		// of dense tensor
		for (int i = 0; i < row; i++)
			data[i] = new int[c];

		//initialize data
		for (int i = 0; i < row; i++)
			{
				for (int j = 0; j < col; j++)
				{
					data[i][j] = 0;
				}
			}
	}

	// insert elements into sparse tensor
	void sparse_insert(int r, int c, int val)
	{

		// invalid entry
		if (r > row-1 || c > col-1)
		{
			cout << "Wrong entry. Start indicies from 0";
		}
		else
		{

			// insert row value
			data[len][0] = r;

			// insert col value
			data[len][1] = c;

			// insert element's value
			data[len][2] = val;

			// increment number of data in tensor
			len++;
		}
	}

	void sparse_add(Tensor2D b)
	{

		// if tensors don't have same dimensions
		if (row != b.row || col != b.col)
		{
			cout << "Tensors can't be added";
		}

		else
		{
			int apos = 0, bpos = 0;
			Tensor2D result(row, col);

			while (apos < len && bpos < b.len)
			{

				// if b's row and col is smaller
				if (data[apos][0] > b.data[bpos][0] ||
					(data[apos][0] == b.data[bpos][0] &&
					 data[apos][1] > b.data[bpos][1]))

				{

					// insert smaller value into result
					result.sparse_insert(b.data[bpos][0],
									 b.data[bpos][1],
									 b.data[bpos][2]);

					bpos++;
				}

				// if a's row and col is smaller
				else if (data[apos][0] < b.data[bpos][0] ||
						 (data[apos][0] == b.data[bpos][0] &&
						  data[apos][1] < b.data[bpos][1]))

				{

					// insert smaller value into result
					result.sparse_insert(data[apos][0],
									 data[apos][1],
									 data[apos][2]);

					apos++;
				}

				else
				{

					// add the values as row and col is same
					int addedval = data[apos][2] +
								   b.data[bpos][2];

					if (addedval != 0)
						result.sparse_insert(data[apos][0],
										 data[apos][1],
										 addedval);
					// then insert
					apos++;
					bpos++;
				}
			}

			// insert remaining elements
			while (apos < len)
				result.sparse_insert(data[apos][0],
								 data[apos][1],
								 data[apos++][2]);

			while (bpos < b.len)
				result.sparse_insert(b.data[bpos][0],
								 b.data[bpos][1],
								 b.data[bpos++][2]);

			// print result
			result.sparse_print();
		}
	}

	void transpose()
	{
        int **temp_data;
        temp_data = new int *[MAX];

		// Array representation
		// of sparse tensor
		//[,0] represents row
		//[,1] represents col
		//[,2] represents value
		for (int i = 0; i < MAX; i++)
			temp_data[i] = new int[3];

        //copy to temp
        for(int i = 0; i < len; i++){
            temp_data[i][0] = data[i][0];
            temp_data[i][1] = data[i][1];
        }
        //swap row and col
        for(int i = 0; i < len; i++){
            data[i][0] = temp_data[i][1];
            data[i][1] = temp_data[i][0];
        }
	}

	void sparse_multiply(Tensor2D b)
	{
		if (col != b.row)
		{
			// Invalid multiplication
			cout << "Can't multiply tensors, Invalid dimensions";
			return;
		}

        convert_sparse_to_dense();
        b.convert_sparse_to_dense();
        dense_multiply(b);
        convert_dense_to_sparse();
        b.convert_dense_to_sparse();
	}

	// printing tensor
	void sparse_print()
	{
		cout << "\nDimension: " << row << "x" << col;
		cout << "\nSparse Tensor: \nRow\tColumn\tValue\n";

		for (int i = 0; i < len; i++)
		{
			cout << data[i][0] << "\t " << data[i][1]
				 << "\t " << data[i][2] << endl;
		}
	}

	//Method to add two dense tensors
	//The calling tensor is updated, the parameter tensor is unchanged
	void dense_add(Tensor2D b)
	{

		// if tensors don't have same dimensions
		if (row != b.row || col != b.col)
		{
			cout << "Tensors can't be added";
		}

		else
		{
			for (int i = 0; i < row; i++)
			{
				for (int j = 0; j < col; j++)
				{
					data[i][j] = (data[i][j] + b.data[i][j]);
				}
			}
		}
	}

	//Method to multiply two dense tensors
	//The calling tensor is updated, the parameter tensor is unchanged
	void dense_multiply(Tensor2D b){
		if (col != b.row)
		{
			// Invalid multiplication
			cout << "Can't multiply tensors, Invalid dimensions";
			return;
		}

		  for(int i = 0; i < row; i++){
    		for(int j = 0; j < b.col; j++){
      			for(int k = 0; k < col; k++){

					data[i][j] += (data[i][k] * b.data[k][j]);
	
       			}
      		}
     	}
	}


	void convert_sparse_to_dense(){

		//Make a temporary tensor to hold values from sparse representation
		int **temp_data;

		//Array of Pointer to make a tensor
		temp_data = new int *[row];

		// Array representation
		// of dense tensor
		for (int i = 0; i < row; i++)
			temp_data[i] = new int[col];

		//initialize temp_data
		for (int i = 0; i < row; i++)
			{
				for (int j = 0; j < col; j++)
				{
					temp_data[i][j] = 0;
				}
			}


		for (int i = 0; i < len; i++)
		{
            
			temp_data[data[i][0]][data[i][1]] = data[i][2];
            
		}

		//Re-initalize data as a 2D dense tensor

		//Array of Pointer to make a tensor
		data = new int *[row];

		// Array representation
		// of dense tensor
		for (int i = 0; i < row; i++)
			data[i] = new int[col];

		//Copy temp_data to data
		for (int i = 0; i < row; i++)
			{
				for (int j = 0; j < col; j++)
				{
					data[i][j] = temp_data[i][j];
				}
			}

	}

	//Method to insert values into a dense matrix
	void dense_insert(int r, int c, int val){
		if(r > row || c > col){
			cout << "Can't insert, Invalid dimensions";
		}
		else{
		data[r][c] = val;
		}
	}

	//Method to multiply a sparse tensor with a dense tensor
	//Must be used on a sparse tensor
	//Calling tensor is now a dense tensor
	void sparse_dense_multiply(Tensor2D b){
		convert_sparse_to_dense();
		dense_multiply(b);
	}


    void print_dense(){
        for (int i = 0; i < row; i++)
			{
                cout << "[";
				for (int j = 0; j < col; j++)
				{
					cout << data[i][j];
                    if(j+1 < col){
                        cout << ", ";
                    }

				}
                cout << "]" << endl;
			}

    }


    void convert_dense_to_sparse(){
        int **temp_data;

		//Array of Pointer to make a tensor
		temp_data = new int *[row];

		// Array representation
		// of dense tensor
		for (int i = 0; i < row; i++)
			temp_data[i] = new int[col];

		//initialize temp_data
		for (int i = 0; i < row; i++)
			{
				for (int j = 0; j < col; j++)
				{
					temp_data[i][j] = data[i][j];
				}
			}
        //Reinitalize using sparse structure
        data = new int *[MAX];

		// Array representation
		// of sparse tensor
		//[,0] represents row
		//[,1] represents col
		//[,2] represents value
		for (int i = 0; i < MAX; i++)
			data[i] = new int[3];

        
        len = 0;
        for(int i = 0; i < row; i++){
            for(int j = 0; j < col; j++){

                if(temp_data[i][j] != 0){
                    sparse_insert(i,j,temp_data[i][j]);
                }

            }
        }


    }





}; //end tensor class 

// Driver Code
int main()
{

	// create two sparse 2D tensors and insert values
	Tensor2D a(4, 4);
	Tensor2D b(4, 4);

	a.sparse_insert(1, 2, 10);
	a.sparse_insert(1, 3, 12);
	a.sparse_insert(2, 3, 5);
	a.sparse_insert(3, 1, 15);
	a.sparse_insert(3, 2, 12);

	b.sparse_insert(1, 3, 8);
	b.sparse_insert(2, 3, 23);
	b.sparse_insert(3, 3, 9);
	b.sparse_insert(3, 1, 20);
	b.sparse_insert(3, 2, 25);

	// Output result
	cout << "Sparse Addition: ";
	a.sparse_add(b);
	cout << "\nSparse Multiplication: ";
	a.sparse_multiply(b);
    a.sparse_print();
	cout << "\nSparse Transpose: ";
	b.transpose();
	b.sparse_print();


    //Testing dense tensor operations
    Tensor2D c(4, 4, 0);
    Tensor2D d(4, 4, 0);

    //Initalize c
    c.dense_insert(0, 0, 1);
    c.dense_insert(0, 1, 2);
    c.dense_insert(0, 2, 3);
    c.dense_insert(0, 3, 4);
    c.dense_insert(1, 0, 5);
    c.dense_insert(1, 1, 6);
    c.dense_insert(1, 2, 7);
    c.dense_insert(1, 3, 8);
    c.dense_insert(2, 0, 9);
    c.dense_insert(2, 1, 1);
    c.dense_insert(2, 2, 2);
    c.dense_insert(2, 3, 3);
    c.dense_insert(3, 0, 4);
    c.dense_insert(3, 1, 4);
    c.dense_insert(3, 2, 4);
    c.dense_insert(3, 3, 4);
    cout << endl;
    cout << "Initalize c: " << endl;
    c.print_dense();

    //Initalize d
    d.dense_insert(0, 0, 6);
    d.dense_insert(0, 1, 5);
    d.dense_insert(0, 2, 1);
    d.dense_insert(0, 3, 2);
    d.dense_insert(1, 0, 1);
    d.dense_insert(1, 1, 3);
    d.dense_insert(1, 2, 1);
    d.dense_insert(1, 3, 4);
    d.dense_insert(2, 0, 9);
    d.dense_insert(2, 1, 1);
    d.dense_insert(2, 2, 2);
    d.dense_insert(2, 3, 3);
    d.dense_insert(3, 0, 2);
    d.dense_insert(3, 1, 1);
    d.dense_insert(3, 2, 2);
    d.dense_insert(3, 3, 1);
    cout << endl;
    cout << "Initialize d: " << endl;
    d.print_dense();
    cout << endl;
    cout << "Dense Addition: " << endl;
	c.dense_add(d);
    c.print_dense();
	cout << "\nDense Multiplication: " <<endl;
	c.dense_multiply(d);
    c.print_dense();
    cout << endl;
    cout << "Sparse-Dense Multiplication: " <<endl;
    b.sparse_dense_multiply(d);
    b.print_dense();
    cout << endl;
}
