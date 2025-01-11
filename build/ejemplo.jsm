var boolean  booleano;

output 4%5%77;
function  boolean bisiesto  (int a)	
{	var string bis;
	output 'Es bisiesto?';
	input bis;
	return ((a % 4 > 0));	
}
function int dias (int m, int a)
{
	var int dd;
	output('di cuantos dias tiene el mes ');
	output m;
	output ' de '; output a;
	input dd;
	if (bisiesto(a)) dd = dd - 1;
	return dd;
}
function  boolean esFechaCorrecta (int d, int m, int a)	
{
	return !(d > dias (m, a));
}
function void demo  (void)	
{

	if (esFechaCorrecta(25, 10, 2024)) output ('OK');
	
}
var boolean  abc;
demo();
