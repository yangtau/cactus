# Cactus

## Code Snippets
```
# this is a comment

[1..10] | map {x->x*x}
        | filter {x->x%2==0}
        | stdout

let echo = map {i->
    puts i
    yield i
}

# greet is a function
let greet = {name->
    puts "hello, ${name}"
}

let filter func = map {i->
    if func i {
        yield i
    } 
}

let take_while func = map {i->
    if func i {
        yield i
    } else {
        close  # keyword to close a stream
    }
}

let take n = zip [1..n] | map {x, _->
    yield x
}

[1..100]
    | echo   # puts and continue the stream
    | filter {x->x%3==0}
    | map    {x->x*x}
    | take 20 # close the stream after take 20 elements
    | stdout

[1..10]
    | zip stdin
    | map {i, s->          
        "${i} ${s}"
      }
    | stdout
```
