// C++ code to perform add, multiply
// and transpose on sparse tensors.
//Also perform add and multiply on dense tensors.
//Can perform multiplication on 1 sparse and 1 dense tensor.
#include <iostream>
using namespace std;

class tensor
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
	tensor(int r, int c)
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
	tensor(int r, int c, int placeholder)
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
	void sparse_insert_2D(int r, int c, int val)
	{

		// invalid entry
		if (r > row || c > col)
		{
			cout << "Wrong entry";
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

	void sparse_add_2D(tensor b)
	{

		// if tensors don't have same dimensions
		if (row != b.row || col != b.col)
		{
			cout << "Tensors can't be added";
		}

		else
		{
			int apos = 0, bpos = 0;
			tensor result(row, col);

			while (apos < len && bpos < b.len)
			{

				// if b's row and col is smaller
				if (data[apos][0] > b.data[bpos][0] ||
					(data[apos][0] == b.data[bpos][0] &&
					 data[apos][1] > b.data[bpos][1]))

				{

					// insert smaller value into result
					result.sparse_insert_2D(b.data[bpos][0],
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
					result.sparse_insert_2D(data[apos][0],
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
						result.sparse_insert_2D(data[apos][0],
										 data[apos][1],
										 addedval);
					// then insert
					apos++;
					bpos++;
				}
			}

			// insert remaining elements
			while (apos < len)
				result.sparse_insert_2D(data[apos][0],
								 data[apos][1],
								 data[apos++][2]);

			while (bpos < b.len)
				result.sparse_insert_2D(b.data[bpos][0],
								 b.data[bpos][1],
								 b.data[bpos++][2]);

			// print result
			result.sparse_print();
		}
	}

	tensor transpose_2D()
	{

		// new tensor with inversed row X col
		tensor result(col, row);

		// same number of elements
		result.len = len;

		// to count number of elements in each column
		int *count = new int[col + 1];

		// initialize all to 0
		for (int i = 1; i <= col; i++)
			count[i] = 0;

		for (int i = 0; i < len; i++)
			count[data[i][1]]++;

		int *index = new int[col + 1];

		// to count number of elements having
		// col smaller than particular i

		// as there is no col with value < 0
		index[0] = 0;

		// initialize rest of the indices
		for (int i = 1; i <= col; i++)

			index[i] = index[i - 1] + count[i - 1];

		for (int i = 0; i < len; i++)
		{

			// insert a data at rpos and
			// increment its value
			int rpos = index[data[i][1]]++;

			// transpose row=col
			result.data[rpos][0] = data[i][1];

			// transpose col=row
			result.data[rpos][1] = data[i][0];

			// same value
			result.data[rpos][2] = data[i][2];
		}

		// the above method ensures
		// sorting of transpose tensor
		// according to row-col value
		return result;
	}

	void sparse_multiply_2D(tensor b)
	{
		if (col != b.row)
		{

			// Invalid multiplication
			cout << "Can't multiply tensors, Invalid dimensions";
			return;
		}

		// transpose b to compare row
		// and col values and to add them at the end
		b = b.transpose_2D();
		int apos, bpos;

		// result tensor of dimension row X b.col
		// however b has been transposed,
		// hence row X b.row
		tensor result(row, b.row);

		// iterate over all elements of A
		for (apos = 0; apos < len;)
		{

			// current row of result tensor
			int r = data[apos][0];

			// iterate over all elements of B
			for (bpos = 0; bpos < b.len;)
			{

				// current column of result tensor
				// data[,0] used as b is transposed
				int c = b.data[bpos][0];

				// temporary pointers created to add all
				// multiplied values to obtain current
				// element of result tensor
				int tempa = apos;
				int tempb = bpos;

				int sum = 0;

				// iterate over all elements with
				// same row and col value
				// to calculate result[r]
				while (tempa < len && data[tempa][0] == r &&
					   tempb < b.len && b.data[tempb][0] == c)
				{
					if (data[tempa][1] < b.data[tempb][1])

						// skip a
						tempa++;

					else if (data[tempa][1] > b.data[tempb][1])

						// skip b
						tempb++;
					else

						// same col, so multiply and increment
						sum += data[tempa++][2] *
							   b.data[tempb++][2];
				}

				// insert sum obtained in result[r]
				// if its not equal to 0
				if (sum != 0)
					result.sparse_insert_2D(r, c, sum);

				while (bpos < b.len &&
					   b.data[bpos][0] == c)

					// jump to next column
					bpos++;
			}
			while (apos < len && data[apos][0] == r)

				// jump to next row
				apos++;
		}
		result.sparse_print();
	}

	// printing tensor
	void sparse_print()
	{
		cout << "\nDimension: " << row << "x" << col;
		cout << "\nSparse Matrix: \nRow\tColumn\tValue\n";

		for (int i = 0; i < len; i++)
		{
			cout << data[i][0] << "\t " << data[i][1]
				 << "\t " << data[i][2] << endl;
		}
	}

	//Method to add two dense tensors
	//The calling tensor is updated, the parameter tensor is unchanged
	void dense_tensor_add_2D(tensor b)
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
	void dense_tensor_multiply_2D(tensor b){
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


	void convert_sparse_to_dense_2D(){

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
			//Stores the value from the sparse representation into temp_data
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
	void dense_insert_2D(int r, int c, int val){
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
	void sparse_dense_multiply_2D(tensor b){
		convert_sparse_to_dense_2D();
		dense_tensor_multiply_2D(b);
	}


}; //end tensor class 

// Driver Code
int main()
{

	// create two sparse 2D tensors and insert values
	tensor a(4, 4);
	tensor b(4, 4);

	a.sparse_insert_2D(1, 2, 10);
	a.sparse_insert_2D(1, 4, 12);
	a.sparse_insert_2D(3, 3, 5);
	a.sparse_insert_2D(4, 1, 15);
	a.sparse_insert_2D(4, 2, 12);

	b.sparse_insert_2D(1, 3, 8);
	b.sparse_insert_2D(2, 4, 23);
	b.sparse_insert_2D(3, 3, 9);
	b.sparse_insert_2D(4, 1, 20);
	b.sparse_insert_2D(4, 2, 25);

	// Output result
	cout << "Addition: ";
	a.sparse_add_2D(b);
	cout << "\nMultiplication: ";
	a.sparse_multiply_2D(b);
	cout << "\nTranspose: ";
	tensor atranspose = a.transpose_2D();
	atranspose.sparse_print();
}
