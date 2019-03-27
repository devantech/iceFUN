/*
 *  
 *  Copyright(C) 2018 Gerald Coe, Devantech Ltd <gerry@devantech.co.uk>
 * 
 *  Permission to use, copy, modify, and/or distribute this software for any purpose with or
 *  without fee is hereby granted, provided that the above copyright notice and 
 *  this permission notice appear in all copies.
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO
 *  THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. 
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL 
 *  DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 *  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 *  CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 */
 
// LedScan takes the four led columns as inputs and outputs them to the led matrix

module Music (
	input clk12MHz, 
	input [7:0] midi,		
	output reg note
	);

	reg [14:0] notetime;		// = 22933;

// Timer register */
    reg [14:0] timer = 15'b0;
	
// increment the note timer
    always @ (posedge clk12MHz) begin
    	if (timer==notetime) begin
    		timer <= 15'b0;
    		if(midi[7:0]==0) note <= note;
    		else note <= !note;
    		end
    	else timer <= timer + 1;
    end


	always @ (posedge clk12MHz) begin
		notetime <= notetime;
		case (midi[7:0])
			8'd60: notetime <= 22933;	// C  C4
			8'd67: notetime <= 15306;	// G  G4
			8'd72: notetime <= 11467;	// c  C5
			8'd74: notetime <= 10216;	// d  D5
			8'd76: notetime <= 9101;	// e  E5
		endcase
	end
	
endmodule



module CloseEncounters (
	input clkNote, 
	input key,
	output reg [7:0] midi
	);	

	reg [2:0] state = 3'b000;
	
	always @ (posedge clkNote) begin
		case (state)
			3'b000:	
				begin
					midi[7:0] <= 0;
					if(key==0) state <= 3'b001;
					else state <= 3'b000;
				end
			3'b001:
				begin
					midi[7:0] <= 74;
					state <= 3'b010;					
				end
			3'b010:
				begin
					midi[7:0] <= 76;
					state <= 3'b011;					
				end
			3'b011:
				begin
					midi[7:0] <= 72;
					state <= 3'b100;					
				end
			3'b100:
				begin
					midi[7:0] <= 60;
					state <= 3'b101;					
				end
			3'b101:
				begin
					midi[7:0] <= 67;
					state <= 3'b110;					
				end
			3'b110:
				begin
					midi[7:0] <= 67;
					state <= 3'b000;					
				end
		endcase
	end
	
endmodule

























