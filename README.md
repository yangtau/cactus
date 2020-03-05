# Cactus

## Code Snippets
```
# this is a comment

# Every thing should be viewed as function.
# The type name after colon, which must begin with capital letter, is the
# return type of the function.

let a: Int32 = 1
let b        = 2 # Int32 for default

let greet = { |name: String|
    puts 'Hello, ${name}'
}

greet # call the function

let xs: Array<Int32> = [1, 2, 3, 4, 5]
let ys = xs->map {|x| x*x}  # ys = [1, 4, 9, 16, 25]
xs->map! {|x| x*x}          # xs = [1, 4, 9, 16, 25]
# definition of map:
#       map: Iterable->Block->Iterable

10->times {
    puts 'Hello'
}


type Tree {
    left: Tree
    right: Tree
    val: Int32
}

```
