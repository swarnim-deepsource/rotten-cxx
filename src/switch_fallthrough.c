void func() {}
int bar() {
    switch (1) {
    case 0: { // [CXX-C1001]
    };
    case 1: { // [CXX-C1001]
    };
    case 2: {
    } break;
    case 3: {
        break;
    }
    case 6: {
    } if (1) {
    } else {
        break;
    };
    case 5: {
    }; 
    switch(0) {
        case 0: break;
        case 1: return;
        case 2: // false negative can't get around without a lot more tedious code
        default: {
            break;
        }
    }
    switch(0) {
        case 0:  // false negative can't get around without a lot more tedious code
        case 1: return;
        case 2:; // [CXX-C1001] // works if a neighbour not child
        default: {
            break;
        }
    }
    break;
    case 4: // [CXX-C1001]
    default: {
    }
    }
}

int foo() {
    t:
    switch (1) {
    case 0: {
        // ..
    } goto t;
    case 1: {
        // ..
    }
        return;
    case 2:
        return;
    default: {
        // ..
    }
    }
}
