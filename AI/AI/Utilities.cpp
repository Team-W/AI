#include "Utilities.h"

// Display Functions
ostream& operator<<(ostream &o, const glm::vec2 &vector)
{
	o.setf(ios::fixed, ios::floatfield);
	o.precision(4);
	o << "["  << vector.x << ", " << vector.y << "]";
	o.unsetf(ios::floatfield);
	return o;
}

ostream& operator<<(ostream &o, const glm::mat4 &matrix)
{
	o.setf(ios::fixed, ios::floatfield);
	o.precision(4);
	o << "---------- MATRIX ----------" << endl;
	for(int j=0; j<4; ++j)
	{
		for(int i=0; i<4; ++i)
		{
			o << matrix[i][j] << "\t";
		}
		o << endl;
	}
	o << "----------------------------" << endl;
	o.unsetf(ios::floatfield);
	return o;
}

// Matrix Operations
void MatrixToArray(GLfloat array[16], const glm::mat4 &model_matrix, const glm::mat4 &view_matrix)
{
	glm::mat4 mv_matrix = view_matrix * model_matrix;

	for(int i=0; i<4; ++i)
	{
		array[4 * i] = mv_matrix[i][0];
		array[4 * i + 1] = mv_matrix[i][1];
		array[4 * i + 2] = mv_matrix[i][2];
		array[4 * i + 3] = mv_matrix[i][3];
	}
}
