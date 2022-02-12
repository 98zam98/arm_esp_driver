
//user edit in the end of the file -- scroll down   ---------------------------------


// prototype start -----------------------------------

u8 if_ok();
void if_start_orders_done();
u8 if_i_main_start_length_eq_loop_length();
void send_start_order();
void send_loop_order();
u8 checking_ok();
void esp_init();
u8 esp_itr();
void init_to_do_func();
void to_do_func();

// prototype end -----------------------------------

u8 i_main=0;

u8 ATE0[]="ATE0";
u8 AT_CWJAP_CUR[]="AT+CWJAP_CUR=\"ABOZYAD5959\",\"Sz@2370sz\"";

u8 AT_CIPSTART[]="AT+CIPSTART=\"TCP\",\"162.253.155.225\",80";
u8 AT_CIPSEND_42[]="AT+CIPSEND=42";

u8 GET_led0[]="GET http://zflutjs2.orgfree.com/led0.txt";
u8 wait_GET_led0[]="";


u8* start_orders[]={ATE0,AT_CWJAP_CUR};
u8 start_orders_done[]={1,1};

u8* loop_orders[]={AT_CIPSTART,AT_CIPSEND_42,GET_led0,wait_GET_led0};

u8 start_cont=1;
u8 loop_cont=0;

u8 start_length=2;
u8 loop_length=4;

u8 i_main_start_length;




u8 this_time=0;
u8 is_OK_k;
u8 is_OK_o;


u8 last_was_OK = 1;




u8 zrx;



u8 if_ok()
{

	if(is_OK_o&&is_OK_k)
	{
		is_OK_o = 0;
		is_OK_k = 0;
		last_was_OK = 1;
		//void_send_string_uart("rec OK");
		//MACRO_send_NEW_LINE_uart();
		i_main++;

		return 1;

	}
	return 0;


}



void if_start_orders_done()
{
	if(start_orders_done[1])
	{
		start_cont=1;
		loop_cont=0;
	}
}

u8 if_i_main_start_length_eq_loop_length()
{
	if(i_main_start_length==loop_length)
	{
		i_main = start_length;

		return 1;
	}
	return 0;
}

void send_start_order()
{

	if(start_cont)
	{
		if(last_was_OK)
		{
			if(start_orders_done[i_main])
			{
				void_send_string_uart(start_orders[i_main]);
				start_orders_done[i_main]=0;
				MACRO_send_NEW_LINE_uart();
			}
		}
	}

}



void send_loop_order()
{
	if(loop_cont)
	{
		if(last_was_OK)
		{
			void_send_string_uart(loop_orders[i_main_start_length]);
			MACRO_send_NEW_LINE_uart();
		}

	}

}

u8 checking_ok()
{
	if(is_OK_o&&zrx=='K')
	{
		is_OK_k = 1;
		return 1;
	}

	if(is_OK_o&&zrx!='K'&&zrx!=0xff)
	{
		is_OK_o = 0;
		return 1;
	}

	if(zrx=='O')
	{
		is_OK_o = 1;
		is_OK_k = 0;
		return 1;
	}
	return 0;
}

u8 checking_rec_data()
{
		if(this_time)
		{
			this_time = 0;

			to_do_func();

			last_was_OK = 1;

			i_main++;
			return 1;
		}
		if(zrx==':')
		{
			this_time = 1;
			return 1;
		}
		return 0;
}

void esp_init()
{

	void_init_uart();

	init_to_do_func();
}
u8 esp_itr()
{
	i_main_start_length = i_main-start_length ;

	// rec ok  --------------------------------------------------------
	if(if_ok())
		return 1;

	// orders -------------------------------------------------------------
	start_cont=0;
	loop_cont=1;
	if_start_orders_done();
	if(if_i_main_start_length_eq_loop_length())
		return 1;
	send_start_order();
	send_loop_order();
	last_was_OK=0;

	// read uart --------------------------------------------------------
	zrx = u8_read_byte_uart();
	if(zrx==0xff)
		return 1;
	//void_send_byte_uart(zrx);

	// check ok --------------------------------------------------------
	if(checking_ok())
		return 1;

	//after rec data check : and digit --------------------------------------------------------------------
	if(checking_rec_data())
		return 1;

	return 0;

}


//user edit start ---------------------------------

u8 led0;

// dont change func name
void init_to_do_func()
{
	//user func edit start ------

	MACRO_ENABLE_PERPH_RCC(RCC_APB2ENR_OFFSET, RCC_APB2ENR_IOPBEN_IO_port_C_clock_enable);


	void_init_output_push_pull_GPIO(C_u2_value_GPIOx_PORTS, 13, OUTPUT_50_MHz_u2_value_GPIOx_PORTS);

	MACRO_CONFIG_high_output_GPIO(C_u2_value_GPIOx_PORTS, 13);

	//user func edit end ------
}

// dont change func name
void to_do_func()
{
	//user func edit start ------
	led0 = zrx;
	led0 -= '0';
	if(led0)
		MACRO_CONFIG_low_output_GPIO(C_u2_value_GPIOx_PORTS, 13);
	else
		MACRO_CONFIG_high_output_GPIO(C_u2_value_GPIOx_PORTS, 13);

	//user func edit end ------
}


//user edit end   ---------------------------------

