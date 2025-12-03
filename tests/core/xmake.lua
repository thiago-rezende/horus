-- core tests
for _, sourcefile in ipairs(os.files('*.c')) do
  local sourcename = path.basename(sourcefile)

  target('tests/core/' .. sourcename)
    set_kind('binary')
    set_default(false)

    add_files(sourcefile)
    add_tests('default')

    add_deps('horus')
    add_packages('cmocka')
end
