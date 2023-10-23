package serport

import (
	"fmt"
	"os"
	"runtime"
	"strings"

	"go.bug.st/serial.v1"
)

func stringInSlice(a string, list []string) bool {
	for _, b := range list {
		if b == a {
			return true
		}
	}
	return false
}

func GetArduinoSerialDevices() []string {
	var ports []string
	if list, err := serial.GetPortsList(); err == nil {
		for _, port := range list {
			if !stringInSlice(port, ports) &&
				(runtime.GOOS != "windows" &&
					strings.Contains(port, "/dev/tty.usb")) {
				ports = append(ports, port)
			}
		}
	} else {
		fmt.Println("\nError: " + err.Error())
		os.Exit(0)
	}

	return ports
}
