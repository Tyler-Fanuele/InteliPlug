package main

import (
	"fmt"
	"sync"
	"time"
)

func worker(toMain chan<- string, toWorker <-chan string, wg *sync.WaitGroup) {
	fmt.Print("working...")
	str := <-toWorker
	for i := 0; i < 10; i++ {
		toMain <- str
		toMain <- "false"
		time.Sleep(time.Second)
	}
	fmt.Println("done")
	//close(toMain)
	//wg.Done()
}

func main() {
	var wg sync.WaitGroup
	wg.Add(1)
	toMain := make(chan string, 100)
	toWorker := make(chan string, 100)
	go worker(toMain, toWorker, &wg)
	toWorker <- "test"
	for {
		select {
		case val := <-toMain:
			fmt.Println(val)
		// default works here if no communication is available
		default:
			// do idle work
		}
	}
	//wg.Wait()

}
