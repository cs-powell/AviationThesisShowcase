from enum import Enum
from enum import StrEnum
class column(StrEnum):
    Altitude_AGL = "AltAGL"
    Altitude_Baro = "AltB"
    Altitude_GPS = "AltGPS"
    Altitude_MSL = "AltMSL"
    Altitude_MSL_Lag_Diff = "AltMSL Lag Diff"

    Amps_1 = "amp1"
    Amps_2 = "amp2"

    Angle_of_Attack_Simple = "AOASimple"
    Barometric_Altitude = "BaroA"
    Calibrated_Airspeed = "CAS"

    COM_1 = "COM1"
    COM_2 = "COM2"
    Coordination_Index = "Coordination Index"
    Course = "CRS"
    Density_Ratio = "DensityRatio"

    Engine_1_CHT_Divergence = "E1 CHT Divergence"
    Engine_1_CHT_1 = "E1 CHT1"
    Engine_1_CHT_2 = "E1 CHT2"
    Engine_1_CHT_3 = "E1 CHT3"
    Engine_1_CHT_4 = "E1 CHT4"

    Engine_1_EGT_Divergence = "E1 EGT Divergence"
    Engine_1_EGT_1 = "E1 EGT1"
    Engine_1_EGT_2 = "E1 EGT2"
    Engine_1_EGT_3 = "E1 EGT3"
    Engine_1_EGT_4 = "E1 EGT4"

    Engine_1_Fuel_Flow = "E1 FFlow"
    Engine_1_Oil_Pressure = "E1 OilP"
    Engine_1_Oil_Temperature = "E1 OilT"
    Engine_1_RPM = "E1 RPM"

    Fuel_Quantity_Left = "FQtyL"
    Fuel_Quantity_Right = "FQtyR"

    Ground_Speed = "GndSpd"

    Horizontal_Alert_Limit = "HAL"
    Horizontal_CDI = "HCDI"
    Heading = "HDG"
    Horizontal_Protection_Level_FD = "HPLfd"
    Horizontal_Protection_Level_WAS = "HPLwas"

    Indicated_Airspeed = "IAS"
    Lateral_Acceleration = "LatAc"
    LOC_I_Index = "LOC-I Index"
    Magnetic_Variation = "MagVar"
    Normal_Acceleration = "NormAc"
    Outside_Air_Temperature = "OAT"

    Pitch = "Pitch"
    Roll = "Roll"

    Stall_Index = "Stall Index"
    True_Airspeed = "TAS"
    Total_Fuel = "Total Fuel"
    Track = "TRK"
    True_Airspeed_FPM = "True Airspeed(ft/min)"

    Volts_1 = "volt1"
    Volts_2 = "volt2"

    Vertical_Protection_Level_WAS = "VPLwas"

    Vertical_Speed = "VSpd"
    Vertical_Speed_Calculated = "VSpd Calculated"
    Vertical_Speed_GPS = "VSpdG"

    Wind_Direction = "WndDr"
    Wind_Speed = "WndSpd"
    
print(column.Roll)



