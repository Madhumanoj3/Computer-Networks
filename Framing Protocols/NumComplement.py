int findComplement(int num) {
    int sum = 0;
    int last_bit_flipped;
    int count = 0;
    while(num != 0){
        last_bit_flipped = (num & 1) == 1 ? 0 : 1;
        sum = sum | (last_bit_flipped << count);
        count++;
        num >>= 1;
    }
    return sum;
}
