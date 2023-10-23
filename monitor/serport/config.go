package serport

import "github.com/jacobsa/go-serial/serial"

func ConnectToSMSFirmware(portName string) serial.OpenOptions {
	return serial.OpenOptions{
		PortName:        portName,
		BaudRate:        115200,
		DataBits:        8,
		StopBits:        1,
		MinimumReadSize: 4,
	}
}
