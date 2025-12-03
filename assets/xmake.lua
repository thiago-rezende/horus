-- slang shaders rules
rule('assets.shaders.slang')
  set_extensions('.slang')

  on_build_file(function (target, sourcefile, opt)
    import('utils.progress')
    import('core.project.depend')

    local targetdir = path.join(target:targetdir(), path.directory(sourcefile))

    os.mkdir(targetdir)

    local targetfile = path.join(targetdir, path.basename(sourcefile) .. '.spv')

    local build = function()
      os.vrunv('slangc', {
        sourcefile,
        '-entry', 'vertex_entrypoint',
        '-entry', 'fragment_entrypoint',
        '-target', 'spirv',
        '-profile', 'spirv_1_4',
        '-emit-spirv-directly',
        '-fvk-use-entrypoint-name',
        '-o', targetfile,
      })

      progress.show(opt.progress, '${color.build.object}slang %s', sourcefile)
    end

    if not os.exists(targetfile) then
      build()
    else
      depend.on_changed(build, {files = {sourcefile}})
    end
  end)

  on_clean(function (target)
    for _, sourcefile in pairs(target:sourcefiles()) do
      local targetdir = path.join(target:targetdir(), path.directory(sourcefile))

      local targetfile = path.join(targetdir, path.basename(sourcefile) .. '.spv')

      os.rm(targetfile)
    end
  end)

-- ktx textures rules
rule('assets.textures.ktx')
  set_extensions('.png')

  on_build_file(function (target, sourcefile, opt)
    import('utils.progress')
    import('core.project.depend')

    local targetdir = path.join(target:targetdir(), path.directory(sourcefile))

    os.mkdir(targetdir)

    local targetfile = path.join(targetdir, path.basename(sourcefile) .. '.ktx2')

    local build = function()
      os.vrunv('ktx', {
        'create',
        '--encode', 'uastc',
        '--assign-tf', 'srgb',
        '--generate-mipmap',
        '--format', 'R8G8B8A8_SRGB',
        sourcefile, targetfile,
      })

      progress.show(opt.progress, '${color.build.object}ktx %s', sourcefile)
    end

    if not os.exists(targetfile) then
      build()
    else
      depend.on_changed(build, {files = {sourcefile}})
    end
  end)

  on_clean(function (target)
    for _, sourcefile in pairs(target:sourcefiles()) do
      local targetdir = path.join(target:targetdir(), path.directory(sourcefile))

      local targetfile = path.join(targetdir, path.basename(sourcefile) .. '.ktx2')

      os.rm(targetfile)
    end
  end)

-- assets target
target('assets')
  -- kind
  set_kind('object')

  -- rules
  add_rules('assets.shaders.slang')
  add_rules('assets.textures.ktx')

  -- shader files
  add_files('shaders/*.slang')

  -- texture files
  add_files('textures/*.png')
  add_files('textures/default/*.png')

