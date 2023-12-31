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

	reader.Reset(reader)
	return line
}

func main() {
	portList := serport.GetArduinoSerialDevices()
	if len(portList) < 1 {
		fmt.Println("No Jessy OS found on serial.")
		os.Exit(0)
	}

	port := serport.OpenFirmwareConnection(
		serport.ConnectToFirmware(portList[0]),
	)

	screen.Clear()
	screen.MoveTopLeft()
	for !strings.HasPrefix(serport.ReadFromFirmwareSerial(port), "---") {
	}

	for {
		str := serport.ReadFromFirmwareSerial(port)

		if strings.Contains(str, "<clear>") {
			screen.Clear()
			screen.MoveTopLeft()

			continue
		} else if strings.Contains(str, "<~>") {
			input := readLine()
			serport.WriteToFirmwareSerial(port, input)

			continue
		}

		fmt.Print(str)
		if strings.HasSuffix(str, " #~ ") {
			input := readLine()
			if input == "exit\n" {
				screen.Clear()
				screen.MoveTopLeft()

				serport.CloseFirmwareSerial(port)
				os.Exit(0)
			}

			serport.WriteToFirmwareSerial(port, input)
		}
	}
}
