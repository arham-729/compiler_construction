int main() {
    int arr[5][10];
    int i;
    int j;
    
    i = 0;
    while (i < 5) {
        j = 0;
        while (j < 10) {
            arr[i][j] = i + j;
            j = j + 1;
        }
        i = i + 1;
    }
    return arr[1][2];
}
