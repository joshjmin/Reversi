fs = import('fs')

tests = [
]

foreach test : tests
    test(test, files('runner.py'), args : [test, exe.full_path()])
endforeach
