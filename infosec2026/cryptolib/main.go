package main

import (
	"fmt"
	"math/rand"
	"time"
)

func powMod(base, exp, mod int64) int64 {
	result := int64(1)
	base = base % mod
	for exp > 0 {
		if exp&1 == 1 {
			result = (result * base) % mod
		}
		base = (base * base) % mod
		exp >>= 1
	}
	return result
}

func isProbablyPrime(n int64, k int) bool {
	if n <= 1 {
		return false
	}
	if n == 2 || n == 3 {
		return true
	}
	if n%2 == 0 {
		return false
	}

	rand.Seed(time.Now().UnixNano())

	for i := 0; i < k; i++ {
		a := rand.Int63n(n-3) + 2
		if powMod(a, n-1, n) != 1 {
			return false
		}
	}

	return true
}

func main() {
	fmt.Println("powMod test:")
	fmt.Println(powMod(5, 7, 13))
	fmt.Println(powMod(4, 8, 6))
	fmt.Println(powMod(7, 9, 4))

	fmt.Println("Ferma test:")
	numbers := []int64{2, 3, 4, 5, 17, 25, 97, 100, 101, 561} //car Mickle
	for _, n := range numbers {
		if isProbablyPrime(n, 100) {
			fmt.Printf("%d probably simple\n", n)
		} else {
			fmt.Printf("%d complex\n", n)
		}
	}
}
