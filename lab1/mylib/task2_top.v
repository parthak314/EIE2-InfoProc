// 0 to 3ff hex values displayed on 3 7-segment displays

module	task2_top	(
		SW,     
		HEX0,
		HEX1,
		HEX2
		);
	input		[9:0]	SW;
	output	[6:0]	HEX0;
	output	[6:0]	HEX1;
	output	[6:0]	HEX2;
	
	reg [3:0] seg0, seg1, seg2;

	always @(*) begin
		 seg0 = SW % 16;  
		 seg1 = (SW / 16) % 16;    
		 seg2 = (SW / 256) % 16;
	end

	hex_to_7seg seg_display0 (
		 .out(HEX0), 
		 .in(seg0)
	);

	hex_to_7seg seg_display1 (
		 .out(HEX1), 
		 .in(seg1)
	);

	hex_to_7seg seg_display2 (
		 .out(HEX2), 
		 .in(seg2)
	);


endmodule