import tensorflow as tf

a = tf.sparse.SparseTensor(indices=[[0, 0], [1, 2]], values=[1, 2], dense_shape=[4, 4])

b = tf.sparse.SparseTensor(indices=[[0, 2], [3, 2]], values=[3, 4], dense_shape=[4, 4])

c = tf.Variable([[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12], [13, 14, 15, 16]])

d = tf.Variable([[1, 0, 0, 0], [0, 2, 0, 0], [0, 0, 3, 0], [0, 0, 0, 4]])

output1 = tf.sparse.add(a,b)
print("\nSparse Addition: \n")
print(output1)

output2 = tf.sparse.sparse_dense_matmul(a, c)
print("\nSparse_Dense multiplication: \n")
print(output2)

output3 = tf.matmul(d, c, a_is_sparse=True)
print("\nDense multiplication with is_sparse flag: \n")
print(output3)

# Work in Progress
# def single_mode_dot(A, B):
#     """
#     Dot product between two rank 2 matrices. Deals automatically with either A
#     or B being sparse.
#     :param A: rank 2 Tensor or SparseTensor.
#     :param B: rank 2 Tensor or SparseTensor.
#     :return: rank 2 Tensor or SparseTensor.
#     """
#     a_sparse = K.is_sparse(A)
#     b_sparse = K.is_sparse(B)
#     if a_sparse and b_sparse:
#         raise ValueError('Sparse x Sparse matmul is not implemented yet.')
#     elif a_sparse:
#         output = tf.sparse.sparse_dense_matmul(A, B)
#     elif b_sparse:
#         output = transpose(
#             tf.sparse_tensor_dense_matmul(
#                 transpose(B), transpose(A)
#             )
#         )
#     else:
#         output = tf.matmul(A, B)

#     return output 