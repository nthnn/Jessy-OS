package main

import (
	"bufio"
	"fmt"
	"jessyos/monitor/serport"
	"os"
	"strings"
)

func readLine() string {
	reader := bufio.NewReader(os.Stdin)
	line, err := reader.ReadString('\n')

	if err != nil {
		fmt.Println("Error: " + err.Error())
	}

	return line
}

func main() {
	portList := serport.GetArduinoSerialDevices()
	if len(portList) < 1 {
		fmt.Println("No Jessy OS found on serial.")
		os.Exit(0)
	}

	port := serport.OpenSMSFirmwareConnection(
		serport.ConnectToSMSFirmware(portList[0]),
	)

	for !strings.HasPrefix(serport.ReadFromFirmwareSerial(port), "---") {
	}

	for {
		str := serport.ReadFromFirmwareSerial(port)

		fmt.Print(str)
		if strings.HasSuffix(str, "#~ ") ||
			str == "<~>" {
			input := readLine()
			serport.WriteToFirmwareSerial(port, input)
		}
	}
}
