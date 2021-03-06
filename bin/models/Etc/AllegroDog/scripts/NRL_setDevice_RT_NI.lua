
removeAllDevices("Body")

local nameDevice = {}

nameDevice[1]  = addDevice("J_FL1", "Joint", "devices\\NRL_enc_RT_J0.dml")
nameDevice[2]  = addDevice("J_FL2", "Joint", "devices\\NRL_enc_RT_J1.dml")
nameDevice[3]  = addDevice("J_FL3", "Joint", "devices\\NRL_enc_RT_J2.dml")
nameDevice[4]  = addDevice("J_FR1", "Joint", "devices\\NRL_enc_RT_J3.dml")
nameDevice[5]  = addDevice("J_FR2", "Joint", "devices\\NRL_enc_RT_J4.dml")
nameDevice[6]  = addDevice("J_FR3", "Joint", "devices\\NRL_enc_RT_J5.dml")
nameDevice[7]  = addDevice("J_HL1", "Joint", "devices\\NRL_enc_RT_J6.dml")
nameDevice[8]  = addDevice("J_HL2", "Joint", "devices\\NRL_enc_RT_J7.dml")
nameDevice[9]  = addDevice("J_HL3", "Joint", "devices\\NRL_enc_RT_J8.dml")
nameDevice[10] = addDevice("J_HR1", "Joint", "devices\\NRL_enc_RT_J9.dml")
nameDevice[11] = addDevice("J_HR2", "Joint", "devices\\NRL_enc_RT_J10.dml")
nameDevice[12] = addDevice("J_HR3", "Joint", "devices\\NRL_enc_RT_J11.dml")

setStringProperty(nameDevice[1],  "Device", "Name", "Enc1")
setStringProperty(nameDevice[2],  "Device", "Name", "Enc2")
setStringProperty(nameDevice[3],  "Device", "Name", "Enc3")
setStringProperty(nameDevice[4],  "Device", "Name", "Enc4")
setStringProperty(nameDevice[5],  "Device", "Name", "Enc5")
setStringProperty(nameDevice[6],  "Device", "Name", "Enc6")
setStringProperty(nameDevice[7],  "Device", "Name", "Enc7")
setStringProperty(nameDevice[8],  "Device", "Name", "Enc8")
setStringProperty(nameDevice[9],  "Device", "Name", "Enc9")
setStringProperty(nameDevice[10], "Device", "Name", "Enc10")
setStringProperty(nameDevice[11], "Device", "Name", "Enc11")
setStringProperty(nameDevice[12], "Device", "Name", "Enc12")

nameDevice[1]  = addDevice("J_FL1", "Joint", "devices\\NRL_motor_RT_J0.dml")
nameDevice[2]  = addDevice("J_FL2", "Joint", "devices\\NRL_motor_RT_J1.dml")
nameDevice[3]  = addDevice("J_FL3", "Joint", "devices\\NRL_motor_RT_J2.dml")
nameDevice[4]  = addDevice("J_FR1", "Joint", "devices\\NRL_motor_RT_J3.dml")
nameDevice[5]  = addDevice("J_FR2", "Joint", "devices\\NRL_motor_RT_J4.dml")
nameDevice[6]  = addDevice("J_FR3", "Joint", "devices\\NRL_motor_RT_J5.dml")
nameDevice[7]  = addDevice("J_HL1", "Joint", "devices\\NRL_motor_RT_J6.dml")
nameDevice[8]  = addDevice("J_HL2", "Joint", "devices\\NRL_motor_RT_J7.dml")
nameDevice[9]  = addDevice("J_HL3", "Joint", "devices\\NRL_motor_RT_J8.dml")
nameDevice[10] = addDevice("J_HR1", "Joint", "devices\\NRL_motor_RT_J9.dml")
nameDevice[11] = addDevice("J_HR2", "Joint", "devices\\NRL_motor_RT_J10.dml")
nameDevice[12] = addDevice("J_HR3", "Joint", "devices\\NRL_motor_RT_J11.dml")

setStringProperty(nameDevice[1],  "Device", "Name", "Mot1")
setStringProperty(nameDevice[2],  "Device", "Name", "Mot2")
setStringProperty(nameDevice[3],  "Device", "Name", "Mot3")
setStringProperty(nameDevice[4],  "Device", "Name", "Mot4")
setStringProperty(nameDevice[5],  "Device", "Name", "Mot5")
setStringProperty(nameDevice[6],  "Device", "Name", "Mot6")
setStringProperty(nameDevice[7],  "Device", "Name", "Mot7")
setStringProperty(nameDevice[8],  "Device", "Name", "Mot8")
setStringProperty(nameDevice[9],  "Device", "Name", "Mot9")
setStringProperty(nameDevice[10], "Device", "Name", "Mot10")
setStringProperty(nameDevice[11], "Device", "Name", "Mot11")
setStringProperty(nameDevice[12], "Device", "Name", "Mot12")

-- add system device
removeDevice("COMM0")
removeDevice("COMM1")
removeDevice("COMM_RS232_3DMGX3")
removeDevice("GyroSensor")
removeDevice("SharedMemory")
removeDevice("RemoteTP_BT")

nameDevice[1] = addDevice("Subsys", "Subsys", "devices\\NRL_NICAN0.dml")
nameDevice[2] = addDevice("Subsys", "Subsys", "devices\\NRL_NICAN1.dml")
nameDevice[3] = addDevice("Subsys", "Subsys", "devices\\COMM_RS232_3DMGX3.dml")
nameDevice[4] = addDevice("Subsys", "Subsys", "devices\\Actual_3DMGX3.dml")
nameDevice[5] = addDevice("Subsys", "Subsys", "devices\\shared_memory_v1.0.dml")
nameDevice[6] = addDevice("Subsys", "Subsys", "devices\\RemoteTP_BT.dml")

setStringProperty(nameDevice[1], "Device", "Name", "COMM0")
setStringProperty(nameDevice[2], "Device", "Name", "COMM1")
setStringProperty(nameDevice[3], "Device", "Name", "COMM_RS232_3DMGX3")
setStringProperty(nameDevice[4], "Device", "Name", "GyroSensor")
setStringProperty(nameDevice[5], "Device", "Name", "SharedMemory")
setStringProperty(nameDevice[6], "Device", "Name", "RemoteTP_BT")
