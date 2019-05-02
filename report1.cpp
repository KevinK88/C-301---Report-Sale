#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstdlib>

using namespace std;

#define max_emp 10
#define max_hours 60.00
#define min_hours 0.0
#define max_rate 99.99
#define min_rate 0.0
#define regular_hours_limit 40.00
#define TAX_RATE 0.3
#define parking 10.0
#define transit 5.0
#define overtime_rate 1.50

 void   input_employee_data(ifstream &infile, string &full_name,double &hours, double &hourly_rate, double &deductions);
 string join_name(string first_name, string last_name );
 void   read_data ( ifstream &infile, string &first_name, string &last_name, double &hours, double &hourly_rate, double &deductions);
 void   split_hours(double hours, double &regular_hours, double &overtime_hours);
 double calculate_gross_pay(double regular_hours, double overtime_hours, double hourly_rate);
 double calculate_tax(double gross_pay, double tax_rate);
 double calculate_net_pay(double gross_pay,double tax,double deductions);
 double compute_sum(double data[], int count );
 double compute_min ( double data[], int count);
 double compute_max ( double data[], int count);
 char   get_yesno (string prompt);
 void   output_brief_summary (int num_emps, double total_gross_pay);
  void   output_payroll_summary_html(ofstream &outfile, int num_emps, string emp_names[], double emp_gross[], double total_gross_pay, double average, double minimum, double maximum);
// void output_payroll_data(ofstream &outfile, int num_emps, string emp_names[], double emp_regular_hours[], double emp_hours_over[],
                          // double emp_rate[], double emp_gross[], double emp_tax[], double emp_deduction[], 
                          //  double emp_net_pay[]);
 //void   sort_employees ( int num_emps, double emp_gross[]);
 //void   output_payroll_summary(ofstream &outfile, int num_emps, string emp_names[], double emp_gross[], double total_gross_pay, double average, double minimum, double maximum);
 void sort_employees ( int num_emps,double emp_gross[],string emp_names[], int &num_passes, int &num_comparisions, int &num_swaps);
 
 int main()
 {
	 int num_emps,
         num_passes,
         num_comparisions,
         num_swaps;
	    
        ofstream outfile;
	    ifstream infile;

	 string first_name,
            last_name,   
            full_name;
	 
     double hours,
		    regular_hours,
		    overtime_hours,
		    hourly_rate,
		    gross_pay,
		    tax,
		    net_pay,
		    deductions;
	 string  emp_names[max_emp];
	 
     double emp_deduction[max_emp],
		    emp_gross[max_emp],
		    emp_regular_hours[max_emp],
		    emp_rate[max_emp],
		    emp_hours_over[max_emp],
		    emp_tax[max_emp],
		    emp_net_pay[max_emp];
	
     double  total_gross_pay,
		     average,
		     minimum,
		     maximum;

	 outfile << fixed << setprecision(2);
	 
	 // Initialize counters and accumulators
	 num_emps = 0;
	 total_gross_pay = 0.0;
	  
     infile.open("c:\\temp\\empdata.txt");
	 
     outfile.open ("c:\\temp\\summary.html");
	 if (!infile.is_open() || !outfile.is_open()){
		 cout << "Can not open the file " << endl;
		 exit(EXIT_FAILURE);
	 }
	read_data (infile, first_name, last_name, hours, hourly_rate, deductions);
	
    while (!infile.eof()){
	
    // Input section
	
   //  input_employee_data(infile, full_name, hours, hourly_rate, deductions);

	 // Processing section
	 split_hours(hours, regular_hours, overtime_hours);
	 gross_pay = calculate_gross_pay(regular_hours, overtime_hours, hourly_rate);
	 tax = calculate_tax(gross_pay, TAX_RATE);
	 net_pay = calculate_net_pay(gross_pay, tax, deductions);

	 // Store the employee's name and gross pay and update total

	 emp_names[num_emps] = first_name + "," + " "+ last_name;
	 emp_gross[num_emps] = gross_pay;
	 emp_regular_hours[num_emps] = regular_hours;
	 emp_rate[num_emps] = hourly_rate;
	 emp_hours_over[num_emps] = overtime_hours;
	 emp_tax[num_emps] = tax;
	 emp_deduction[num_emps] = deductions;
	 emp_net_pay[num_emps] = net_pay;
	 total_gross_pay = total_gross_pay + gross_pay;
	 num_emps++;
     read_data ( infile, first_name, last_name, hours, hourly_rate, deductions);
 }
 //Display brief summary
     
// output_brief_summary ( num_emps, total_gross_pay);
 
//Display full payroll summary
 if ( num_emps > 0)
 {
     total_gross_pay = compute_sum(emp_gross, num_emps );
     average = total_gross_pay / num_emps;
     minimum =  compute_min ( emp_gross, num_emps);
     maximum =  compute_max ( emp_gross, num_emps);
 }
 else{

     total_gross_pay = 0.0;
     average = 0.0;
     minimum = 0.0;
     maximum = 0.0;
 }
 
 //output_payroll_data( outfile, num_emps, emp_names, emp_regular_hours , emp_hours_over ,emp_rate , emp_gross , emp_tax , emp_deduction , emp_net_pay );
 sort_employees ( num_emps, emp_gross , emp_names, num_passes, num_comparisions, num_swaps);
  output_payroll_summary_html( outfile,  num_emps, emp_names , emp_gross ,  total_gross_pay, average, minimum, maximum);

 outfile.close();
 infile.close();
}


void input_employee_data(ifstream &infile, string &full_name, double &hours, double &hourly_rate, double &deductions )
{
	
    string	first_name,
	        last_name;
   
	infile >> first_name >> last_name;
    
    full_name = join_name(first_name, last_name);
	infile >> hours;
	infile >> hourly_rate;
	infile >> deductions;
		
   
 }

string join_name(string first_name, string last_name )
{
    string full_name;
    full_name = last_name + ", " + first_name;
	return full_name;
}
      


void split_hours(double hours, double &regular_hours, double &overtime_hours)
{
	if (hours >= regular_hours_limit)

	{
		overtime_hours = hours - regular_hours_limit;

	}

	else if (hours <= regular_hours_limit)

	{
		overtime_hours = 0.0;
	}

	regular_hours = hours - overtime_hours;
}


double calculate_tax(double gross_pay, double tax_rate)
{
    double tax;
	tax = tax_rate * gross_pay;
    return tax;
}



double calculate_gross_pay(double regular_hours, double overtime_hours, double hourly_rate)
{
    double gross_pay;
	gross_pay = (regular_hours * hourly_rate) + (overtime_hours * (hourly_rate * overtime_rate));
    return gross_pay;
}



double calculate_net_pay(double gross_pay,double tax,double deductions)
{
	double net_pay;
	net_pay = gross_pay - tax - deductions;
    return net_pay;
}

/*void output_payroll_data(ofstream &outfile, int num_emps, string emp_names[], double emp_regular_hours[], double emp_hours_over[],
                            double emp_rate[], double emp_gross[], double emp_tax[], double emp_deduction[], 
                           double emp_net_pay[])
{
    int i;
                cout << endl;
                 
                outfile << "                      Reg.   Ovt.   Hourly  Gross                    Net     " << endl;
                outfile << "Name                  Hours  Hours  Rate    Pay      Taxes   Deduct  Pay     " << endl;
                outfile << "====================  =====  =====  ======  =======  ======  ======  ======= " << endl;
                for ( i = 0; i < num_emps; i++){
                outfile << left << setw(19) <<  emp_names[i] <<"  " ;
                outfile << right ;
                outfile << setw(6) << emp_regular_hours[i] ;
                outfile << setw(7) << emp_hours_over[i] ;
                outfile << setw(8) << emp_rate[i] ;
                outfile << setw(9) << emp_gross[i] ;
                outfile << setw(8) << emp_tax[i];
                outfile << setw(8) << emp_deduction[i] ;
                outfile << setw(9) << emp_net_pay[i] << endl;
                outfile << endl;
   }
}
*/


//Display brief summary
void output_brief_summary ( int num_emps, double total_gross_pay)
{
    cout << endl;    
    cout << "Employee processed: " << num_emps << endl;
    cout << "Total: $" << total_gross_pay << endl;
}

double compute_sum(double data[], int count )
{
    double sum = 0;
    for (int i = 0; i < count; i++){
        sum = sum + data[i];
     }
    return sum;
}

double compute_min ( double data[], int count)
{
    double minimum;
    int i;

    minimum = data[0];
    for ( i = 1; i < count; i++){
    if (data[i] < minimum){
        minimum = data[i];
        }
    }
return minimum;
}


double compute_max( double data[], int count)
{
    double maximum;
    int i;

    maximum = data[0];
    for ( i = 1; i < count; i++){
    if (data[i] > maximum){
        maximum = data[i];
        }
    }
return maximum;
}

/*void output_payroll_summary(ofstream &outfile, int num_emps, string emp_names[],double emp_gross[], double total_gross_pay, double average, double minimum, double maximum)
{
int i;
                outfile << endl;
                outfile << endl;
                outfile << "==============================" << endl;
                outfile << "   Employee Payroll Summary   " << endl;
                outfile << endl;
                outfile << "                      Gross   " << endl;
                outfile << "Name                   pay    " << endl;
                outfile << "==============================" << endl;
               
                if(num_emps > 0){
                for ( i = 0; i < num_emps; i++){ 
                    outfile << left << setw(20) << emp_names[i];
                    outfile << right << setw(8) << emp_gross[i] << endl;
                }   
                }
                outfile << endl;
                outfile << "Total gross pay  : $ " << total_gross_pay << endl;
                outfile << "Average gross pay: $ " << average << endl;
                outfile << "Maximum gross pay: $ " << minimum << endl;
                outfile << "Minimum gross pay: $ " << maximum << endl;


}
*/
void read_data ( ifstream &infile, string &first_name, string &last_name, double &hours, double &hourly_rate, double &deductions)
{
infile >> first_name >> last_name >> hours >> hourly_rate >> deductions;
}


void sort_employees ( int num_emps,double emp_gross[], string emp_names[],int &num_passes, int &num_comparisions, int &num_swaps)
{
    int i,
        pass;
    string temp_names;
    double temp_gross;
     num_passes = num_comparisions = num_swaps = 0;
    for (pass = 1; pass < num_emps; pass++)
    {
            num_passes ++;
            for (i =0; i < num_emps - 1; i++)
        {
                    num_comparisions++;
                     if (emp_gross[i] < emp_gross[i+1]){
                    temp_gross = emp_gross[i];
                    emp_gross[i] = emp_gross[i+1];
                    emp_gross[i+1] = temp_gross;    
                    
                    temp_names =  emp_names[i];
                    emp_names[i] = emp_names[i+1];
                    emp_names[i+1] = temp_names;
                         
                     
                }
         }
    }
}
 
void  output_payroll_summary_html(ofstream &outfile, int num_emps, string emp_names[], double emp_gross[], double total_gross_pay, double average, double minimum, double maximum)
 {
    
                outfile << "<!doctype html>" << endl;
                outfile << "<html lang='en'>" << endl;
                outfile << "<head>" << endl;
                outfile << "    <meta charset='utf-8'/>" << endl;
                outfile << "    <title>Inventory Report</title>" << endl;
                outfile << "    <style>" << endl;
                outfile << "        table, th, td { border: 1px solid black }" << endl;
                outfile << "    </style>" << endl;
                outfile << "</head>" << endl; 
                outfile << "<body>" << endl;
                outfile << "    <h1>Employee Payroll Summary</h1>" << endl;
                outfile << "    <table style='border: 1px solid black'>" << endl; 
                outfile << "        <tr>" << endl;
                outfile << "            <th>Name </th>" << endl;
                outfile << "            <th>Gross pay</th>" << endl;
                outfile << "        </tr>" << endl; 
                outfile << "        <tr>" << endl;
                for (int i = 0; i< num_emps; i++){
                outfile << "            <td>" << emp_names[i] << "</td>" ;
                outfile << "            <td>" << emp_gross[i] << "</td>";
                outfile <<  "        <tr>" << endl;
                }
                outfile << "            <th>Total gross pay </th>" << endl;
               outfile << "            <td>" << total_gross_pay << "</td>" ;
               outfile << "        </tr>" << endl;
               outfile << "            <th>Average gross pay</th>" << endl;
               outfile << "            <td>" << average << "</td>" ;
               outfile << "        </tr>" << endl;
               outfile << "            <th>Maximum gross pay</th>" << endl;
                outfile << "            <td>" << minimum << "</td>" ;
                outfile << "        </tr>" << endl;
                outfile << "            <th>Minimum gross pay</th>" << endl;
                outfile << "            <td>" << maximum << "</td>" ;
                outfile << "        </tr>" << endl;
                outfile << "    </table>" << endl;
                outfile << "</body>" << endl;
                outfile << "</html>" << endl;
 }