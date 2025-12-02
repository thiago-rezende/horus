-- math tests
for _, file in ipairs(os.files('*.c')) do
  local name = path.basename(file)

  target('math/' .. name)
    set_kind('binary')
    set_default(false)

    add_files(file)
    add_tests('default')

    add_deps('horus')
    add_packages('cmocka')
end
