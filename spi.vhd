----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 12/03/2023 05:53:00 PM
-- Design Name: 
-- Module Name: IOBUFi - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity SPI is
generic(
FREQ_HZ      : string := "10000000";
IBSTRBYTELRN : integer :=1
);
    Port ( 
            clk      : in std_logic:='0';

           s_io0_o  : in STD_LOGIC:='0';
           s_io1_i  : out STD_LOGIC:='0';
           s_ss2_o  : in STD_LOGIC:='0';
           pll_lock : in STD_LOGIC:='0';
                      
           s_sck_o : in STD_LOGIC:='0';
       pin_sclk : out STD_LOGIC:='0';
           s_ss_o : in STD_LOGIC:='0';
           s_ss_i : out STD_LOGIC:='1';           
           s_ss_t : in STD_LOGIC:='0';           
       pin_csb : out STD_LOGIC:='0';                             
  m_tri_o  : out  std_logic:='0';
  m_tri_t  : out  std_logic:='0';    
  m_tri_i  : in   std_logic:='0';  

  m_ioupdate_o  : out  std_logic:='0';
  m_ioupdate_t  : out  std_logic:='0';    
  m_ioupdate_i  : in   std_logic:='0';
    
  s_b_tri_o  : in   std_logic_vector(15 downto 0):=x"00";
  s_b_tri_t  : in   std_logic_vector(15 downto 0):=x"00";    
  s_b_tri_i  : out  std_logic_vector(15 downto 0):=x"0003";  
  
   debug : out std_logic_vector(3 downto 0):="0000"         
           );
end SPI;

architecture Behavioral of SPI is
ATTRIBUTE X_INTERFACE_INFO : STRING;
ATTRIBUTE X_INTERFACE_PARAMETER : STRING;
ATTRIBUTE X_INTERFACE_MODE      : STRING;
  ATTRIBUTE X_INTERFACE_INFO of m_tri_t: SIGNAL is "xilinx.com:interface:gpio:1.0 sdio TRI_T";
  ATTRIBUTE X_INTERFACE_INFO of m_tri_o: SIGNAL is "xilinx.com:interface:gpio:1.0 sdio TRI_O";
  ATTRIBUTE X_INTERFACE_INFO of m_tri_i: SIGNAL is "xilinx.com:interface:gpio:1.0 sdio TRI_I";
  
  ATTRIBUTE X_INTERFACE_INFO of m_ioupdate_t: SIGNAL is "xilinx.com:interface:gpio:1.0 ioupdate TRI_T";
  ATTRIBUTE X_INTERFACE_INFO of m_ioupdate_o: SIGNAL is "xilinx.com:interface:gpio:1.0 ioupdate TRI_O";
  ATTRIBUTE X_INTERFACE_INFO of m_ioupdate_i: SIGNAL is "xilinx.com:interface:gpio:1.0 ioupdate TRI_I";
  
  ATTRIBUTE X_INTERFACE_INFO of s_b_tri_t: SIGNAL is "xilinx.com:interface:gpio:1.0 B TRI_T";
  ATTRIBUTE X_INTERFACE_INFO of s_b_tri_o: SIGNAL is "xilinx.com:interface:gpio:1.0 B TRI_O";
  ATTRIBUTE X_INTERFACE_INFO of s_b_tri_i: SIGNAL is "xilinx.com:interface:gpio:1.0 B TRI_I";  
ATTRIBUTE X_INTERFACE_MODE   of s_b_tri_o: SIGNAL is "slave";
  
ATTRIBUTE X_INTERFACE_MODE   of s_sck_o: SIGNAL is "slave";  
  ATTRIBUTE X_INTERFACE_INFO of s_sck_o: SIGNAL is "xilinx.com:interface:spi:1.0 SPI SCK_o";
ATTRIBUTE X_INTERFACE_INFO of s_ss_t: SIGNAL is "xilinx.com:interface:spi:1.0 SPI SS_T";  
--ATTRIBUTE X_INTERFACE_PARAMETER of s_sck_o: SIGNAL is "FREQ_HZ FREQ_HZ";  
--  ATTRIBUTE X_INTERFACE_PARAMETER of pin_sclk: SIGNAL is "FREQ_HZ FREQ_HZ";  
ATTRIBUTE X_INTERFACE_INFO of s_io0_o: SIGNAL is "xilinx.com:interface:spi:1.0 SPI io0_o";
ATTRIBUTE X_INTERFACE_INFO of s_ss_o: SIGNAL is "xilinx.com:interface:spi:1.0 SPI SS_o";
ATTRIBUTE X_INTERFACE_INFO of s_ss_i: SIGNAL is "xilinx.com:interface:spi:1.0 SPI SS_i";
ATTRIBUTE X_INTERFACE_INFO of s_io1_i: SIGNAL is "xilinx.com:interface:spi:1.0 SPI io1_i";
ATTRIBUTE X_INTERFACE_INFO of s_ss2_o: SIGNAL is "xilinx.com:interface:spi_rtl:1.0 SPI SS2_o";
  
begin
s_io1_i<=m_tri_i;
m_tri_o<=s_io0_o;
m_tri_t <=s_b_tri_o(1);      -- sdio direction
debug(0)<=s_b_tri_o(1);
pin_csb<=s_ss_o;
pin_sclk<=s_sck_o;

m_ioupdate_o<=s_b_tri_o(0);  -- ioupdate
m_ioupdate_t<=s_b_tri_t(0);  -- ioupdate direction

s_b_tri_i(4)<=pll_lock;      -- EMIO pin

process(clk)
    variable t : std_logic;
begin
if rising_edge(clk) then

end if;
end process;

end Behavioral;
