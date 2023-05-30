

require 'console_io_test'


class ExceptionRestoreTest < SubtestBase
    def name
        'Exception restore'
    end

    def run(qemu_out, _qemu_in)
        expect_or_raise(qemu_out, 'Back from system call!')
    end
end

##--------------------------------------------------------------------------------------------------
## Test registration
##--------------------------------------------------------------------------------------------------
def subtest_collection
    [ExceptionRestoreTest.new]
end
