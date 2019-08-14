#include <math.h>
#include <stdio.h>

void ascii_plot(
	FLOAT_TYPE position,
	FLOAT_TYPE velocity
)
{
	FLOAT_TYPE thresh = .1;
	char row[] = ":                                                                                  :";
	int space = sizeof(row)/sizeof(row[0]) - 1;
	int pos = round((position*space + space)/2) + 1;
	if ( pos < 0 )
		row[0] = '<';
	else if ( pos > space-1 )
		row[space-1] = '>';
	else if (velocity < -thresh)
		row[pos] = '/';
	else if (velocity > thresh)
		row[pos] = '\\';
	else
		row[pos] = '|';
	printf("%s\n",row);
}

