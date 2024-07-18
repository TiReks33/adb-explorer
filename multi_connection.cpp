#include "multi_connection.h"
#include "db_connection.h"

void multi_connection::delete_sql_connection()
{
    if(this->con_name_!=""){
        db_connection::close_con(this->con_name_);
        this->multi_con_names_.removeAll(this->con_name_);
    }
    if(this->con_counter_>0)
        --this->con_counter_;
    if(this->con_counter_==0)
        this->unique_number_=0;
}
