package main

import (
	"bufio"
	"fmt"
	"jessyos/monitor/serport"
	"os"
	"strings"

	"github.com/inancgumus/screen"
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

	screen.Clear()
	screen.MoveTopLeft()
	for !strings.HasPrefix(serport.ReadFromFirmwareSerial(port), "---") {
	}

	for {
		str := serport.ReadFromFirmwareSerial(port)
		if str == "<clear>\r\n" {
			screen.Clear()
			screen.MoveTopLeft()

			continue
		} else if strings.Contains(str, "<~>") {
			input := readLine()
			serport.WriteToFirmwareSerial(port, input+"\n")

			str = strings.Trim(str, "\n")
			if strings.HasPrefix(str, "<~>") &&
				strings.HasSuffix(str, "<~>") &&
				len(str) == 3 {
				continue
			}
		}

		fmt.Print(str)
		if strings.HasSuffix(str, "#~ ") {
			input := readLine()
			serport.WriteToFirmwareSerial(port, input)
		}
	}
}
