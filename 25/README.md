
For the final task this year, which - as is traditional - came in just one part, we had to break what was basically a [Diffie-Hellman key exchange](https://en.wikipedia.org/wiki/Diffie%E2%80%93Hellman_key_exchange) with very small, brute forceable numbers.

I was so glad I had that multiplicative inverse function from day 13 lying around that I immediately used it to determine the loop number:

```cpp
std::vector<long long> powers_of_7;
powers_of_7.resize(100000000);
std::generate(std::begin(powers_of_7),std::end(powers_of_7),[curr = static_cast<long long>(1)]() mutable { auto ret_val = curr; curr=curr*7%modulus; return ret_val; });

const auto which_power_of_7 = [&](long long n)
{
	return std::find(std::begin(powers_of_7),std::end(powers_of_7),n)-std::begin(powers_of_7)+1;
};

const auto find_factor = [&](long long key)
{
	const auto inv = multiplicative_inverse(subject_number,modulus);
	auto n = (inv*key)%modulus;
	
	return which_power_of_7(n);
};
```

Using that result, I could simply execute the algorithm as described to obtain the unknown key:

```cpp
const auto transform_with = [](long long subject_number, long long loop_size)
{
	long long value = 1;
	
	while(loop_size--)
		value=value*subject_number%modulus;
	
	return value;
};
```

Turns out, however, that this is incredibly wasteful and instead of precomputing all those powers, I could simply do the same computations with the known key whilst counting the iterations:

```cpp
const auto find_factor = [&](long long key)
{
	long long value = 1, loop = 0;
	while(value!=key)
	{
		value*=subject_number;
		value%=modulus;
		++loop;
	}
	return loop;
};
```
