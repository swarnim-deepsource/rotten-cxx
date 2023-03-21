
int main() {
    int items[10] = {};
    for (int i = 0; i < 10; i++) {
        // increment items if touched
        int item = items[i++]++;
        // use the `og` value of item
        // ..
    }
}
