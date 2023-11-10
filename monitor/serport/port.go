package serport

import (
	"fmt"
	"io"
	"os"
	"time"

	"github.com/jacobsa/go-serial/serial"
)

func OpenFirmwareConnection(options serial.OpenOptions) io.ReadWriteCloser {
	port, err := serial.Open(options)
	if err != nil {
		fmt.Println("Error: " + err.Error())
		os.Exit(0)
	}

	return port
}

func WriteToFirmwareSerial(portStream io.ReadWriteCloser, content string) {
	_, err := portStream.Write([]byte(content))

	if err != nil {
		fmt.Println("Error: " + err.Error())
		os.Exit(0)
	}

	time.Sleep(200)
}

func ReadFromFirmwareSerial(portStream io.ReadWriteCloser) string {
	buf := make([]byte, 64*1024)
	n, err := portStream.Read(buf)

	if err != nil {
		fmt.Println("\nError: " + err.Error())
		os.Exit(0)
	}

	return string(buf[:n])
}

func CloseFirmwareSerial(portStream io.ReadWriteCloser) {
	portStream.Close()
}
