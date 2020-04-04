# lll

## Code Snippets
```
# this is a comment

[1..10] | map {x->x*x}
        | filter {x->x%2==0}
        | stdout

let greet = {name->
    puts "hello, ${name}"
}

[1..100]
    | filter {x->x%3==0}
    | map    {x->x*x}
    | take 20 # close the stream after take 20 elements
    |> first_twenty_s
# |> creates a stream
first_twenty_s | stdout

[1..10]
    | zip stdin
    | map {i, s->"${i} ${s}"}
    | stdout
```
