(module
  (type (;0;) (func (param i32) (result i32)))
  (type (;1;) (func (param i32)))
  (type (;2;) (func (result i32)))
  (type (;3;) (func (param i32 i32)))
  (import "env" "memory" (memory (;0;) 256 256))
  (import "env" "table" (table (;0;) 2 2 anyfunc))
  (import "env" "tableBase" (global (;0;) i32))
  (import "env" "DYNAMICTOP_PTR" (global (;1;) i32))
  (import "env" "STACKTOP" (global (;2;) i32))
  (import "env" "abort" (func (;0;) (type 1)))
  (import "env" "enlargeMemory" (func (;1;) (type 2)))
  (import "env" "getTotalMemory" (func (;2;) (type 2)))
  (import "env" "abortOnCannotGrowMemory" (func (;3;) (type 2)))
  (import "env" "___setErrNo" (func (;4;) (type 1)))
  (func (;5;) (type 0) (param i32) (result i32)
    (local i32)
    get_local 0
    i32.const 0
    i32.gt_s
    get_global 3
    i32.load
    tee_local 1
    get_local 0
    i32.add
    tee_local 0
    get_local 1
    i32.lt_s
    i32.and
    get_local 0
    i32.const 0
    i32.lt_s
    i32.or
    if  ;; label = @1
      call 3
      drop
      i32.const 12
      call 4
      i32.const -1
      return
    end
    get_global 3
    get_local 0
    i32.store
    get_local 0
    call 2
    i32.gt_s
    if  ;; label = @1
      call 1
      i32.eqz
      if  ;; label = @2
        get_global 3
        get_local 1
        i32.store
        i32.const 12
        call 4
        i32.const -1
        return
      end
    end
    get_local 1)
  (func (;6;) (type 0) (param i32) (result i32)
    (local i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32)
    block  ;; label = @1
      block  ;; label = @2
        block  ;; label = @3
          get_global 4
          set_local 1
          get_global 4
          i32.const 16
          i32.add
          set_global 4
          get_local 1
          set_local 10
          block  ;; label = @4
            get_local 0
            i32.const 245
            i32.lt_u
            if  ;; label = @5
              get_local 0
              i32.const 11
              i32.add
              i32.const -8
              i32.and
              set_local 1
              i32.const 1028
              i32.load
              tee_local 6
              get_local 0
              i32.const 11
              i32.lt_u
              if (result i32)  ;; label = @6
                i32.const 16
              else
                get_local 1
              end
              tee_local 0
              i32.const 3
              i32.shr_u
              tee_local 1
              i32.shr_u
              tee_local 2
              i32.const 3
              i32.and
              if  ;; label = @6
                get_local 2
                i32.const 1
                i32.and
                i32.const 1
                i32.xor
                get_local 1
                i32.add
                tee_local 0
                i32.const 3
                i32.shl
                i32.const 1068
                i32.add
                tee_local 1
                i32.const 8
                i32.add
                tee_local 4
                i32.load
                tee_local 2
                i32.const 8
                i32.add
                tee_local 5
                i32.load
                tee_local 3
                get_local 1
                i32.eq
                if  ;; label = @7
                  i32.const 1028
                  get_local 6
                  i32.const 1
                  get_local 0
                  i32.shl
                  i32.const -1
                  i32.xor
                  i32.and
                  i32.store
                else
                  get_local 3
                  get_local 1
                  i32.store offset=12
                  get_local 4
                  get_local 3
                  i32.store
                end
                get_local 2
                get_local 0
                i32.const 3
                i32.shl
                tee_local 0
                i32.const 3
                i32.or
                i32.store offset=4
                get_local 2
                get_local 0
                i32.add
                i32.const 4
                i32.add
                tee_local 0
                get_local 0
                i32.load
                i32.const 1
                i32.or
                i32.store
                get_local 10
                set_global 4
                get_local 5
                return
              end
              get_local 0
              i32.const 1036
              i32.load
              tee_local 7
              i32.gt_u
              if  ;; label = @6
                get_local 2
                if  ;; label = @7
                  get_local 2
                  get_local 1
                  i32.shl
                  i32.const 2
                  get_local 1
                  i32.shl
                  tee_local 1
                  i32.const 0
                  get_local 1
                  i32.sub
                  i32.or
                  i32.and
                  tee_local 1
                  i32.const 0
                  get_local 1
                  i32.sub
                  i32.and
                  i32.const -1
                  i32.add
                  tee_local 2
                  i32.const 12
                  i32.shr_u
                  i32.const 16
                  i32.and
                  set_local 1
                  get_local 2
                  get_local 1
                  i32.shr_u
                  tee_local 2
                  i32.const 5
                  i32.shr_u
                  i32.const 8
                  i32.and
                  tee_local 3
                  get_local 1
                  i32.or
                  get_local 2
                  get_local 3
                  i32.shr_u
                  tee_local 1
                  i32.const 2
                  i32.shr_u
                  i32.const 4
                  i32.and
                  tee_local 2
                  i32.or
                  get_local 1
                  get_local 2
                  i32.shr_u
                  tee_local 1
                  i32.const 1
                  i32.shr_u
                  i32.const 2
                  i32.and
                  tee_local 2
                  i32.or
                  get_local 1
                  get_local 2
                  i32.shr_u
                  tee_local 1
                  i32.const 1
                  i32.shr_u
                  i32.const 1
                  i32.and
                  tee_local 2
                  i32.or
                  get_local 1
                  get_local 2
                  i32.shr_u
                  i32.add
                  tee_local 3
                  i32.const 3
                  i32.shl
                  i32.const 1068
                  i32.add
                  tee_local 1
                  i32.const 8
                  i32.add
                  tee_local 5
                  i32.load
                  tee_local 2
                  i32.const 8
                  i32.add
                  tee_local 8
                  i32.load
                  tee_local 4
                  get_local 1
                  i32.eq
                  if  ;; label = @8
                    i32.const 1028
                    get_local 6
                    i32.const 1
                    get_local 3
                    i32.shl
                    i32.const -1
                    i32.xor
                    i32.and
                    tee_local 1
                    i32.store
                  else
                    get_local 4
                    get_local 1
                    i32.store offset=12
                    get_local 5
                    get_local 4
                    i32.store
                    get_local 6
                    set_local 1
                  end
                  get_local 2
                  get_local 0
                  i32.const 3
                  i32.or
                  i32.store offset=4
                  get_local 2
                  get_local 0
                  i32.add
                  tee_local 6
                  get_local 3
                  i32.const 3
                  i32.shl
                  tee_local 3
                  get_local 0
                  i32.sub
                  tee_local 4
                  i32.const 1
                  i32.or
                  i32.store offset=4
                  get_local 2
                  get_local 3
                  i32.add
                  get_local 4
                  i32.store
                  get_local 7
                  if  ;; label = @8
                    i32.const 1048
                    i32.load
                    set_local 3
                    get_local 7
                    i32.const 3
                    i32.shr_u
                    tee_local 2
                    i32.const 3
                    i32.shl
                    i32.const 1068
                    i32.add
                    set_local 0
                    get_local 1
                    i32.const 1
                    get_local 2
                    i32.shl
                    tee_local 2
                    i32.and
                    if (result i32)  ;; label = @9
                      get_local 0
                      i32.const 8
                      i32.add
                      tee_local 2
                      i32.load
                    else
                      i32.const 1028
                      get_local 1
                      get_local 2
                      i32.or
                      i32.store
                      get_local 0
                      i32.const 8
                      i32.add
                      set_local 2
                      get_local 0
                    end
                    set_local 1
                    get_local 2
                    get_local 3
                    i32.store
                    get_local 1
                    get_local 3
                    i32.store offset=12
                    get_local 3
                    get_local 1
                    i32.store offset=8
                    get_local 3
                    get_local 0
                    i32.store offset=12
                  end
                  i32.const 1036
                  get_local 4
                  i32.store
                  i32.const 1048
                  get_local 6
                  i32.store
                  get_local 10
                  set_global 4
                  get_local 8
                  return
                end
                i32.const 1032
                i32.load
                tee_local 12
                if  ;; label = @7
                  get_local 12
                  i32.const 0
                  get_local 12
                  i32.sub
                  i32.and
                  i32.const -1
                  i32.add
                  tee_local 2
                  i32.const 12
                  i32.shr_u
                  i32.const 16
                  i32.and
                  set_local 1
                  get_local 2
                  get_local 1
                  i32.shr_u
                  tee_local 2
                  i32.const 5
                  i32.shr_u
                  i32.const 8
                  i32.and
                  tee_local 3
                  get_local 1
                  i32.or
                  get_local 2
                  get_local 3
                  i32.shr_u
                  tee_local 1
                  i32.const 2
                  i32.shr_u
                  i32.const 4
                  i32.and
                  tee_local 2
                  i32.or
                  get_local 1
                  get_local 2
                  i32.shr_u
                  tee_local 1
                  i32.const 1
                  i32.shr_u
                  i32.const 2
                  i32.and
                  tee_local 2
                  i32.or
                  get_local 1
                  get_local 2
                  i32.shr_u
                  tee_local 1
                  i32.const 1
                  i32.shr_u
                  i32.const 1
                  i32.and
                  tee_local 2
                  i32.or
                  get_local 1
                  get_local 2
                  i32.shr_u
                  i32.add
                  i32.const 2
                  i32.shl
                  i32.const 1332
                  i32.add
                  i32.load
                  tee_local 3
                  set_local 1
                  get_local 3
                  i32.load offset=4
                  i32.const -8
                  i32.and
                  get_local 0
                  i32.sub
                  set_local 4
                  loop  ;; label = @8
                    block  ;; label = @9
                      get_local 1
                      i32.load offset=16
                      tee_local 2
                      if  ;; label = @10
                        get_local 2
                        set_local 1
                      else
                        get_local 1
                        i32.load offset=20
                        tee_local 1
                        i32.eqz
                        br_if 1 (;@9;)
                      end
                      get_local 1
                      i32.load offset=4
                      i32.const -8
                      i32.and
                      get_local 0
                      i32.sub
                      tee_local 2
                      get_local 4
                      i32.lt_u
                      tee_local 5
                      i32.eqz
                      if  ;; label = @10
                        get_local 4
                        set_local 2
                      end
                      get_local 5
                      if  ;; label = @10
                        get_local 1
                        set_local 3
                      end
                      get_local 2
                      set_local 4
                      br 1 (;@8;)
                    end
                  end
                  get_local 3
                  get_local 0
                  i32.add
                  tee_local 11
                  get_local 3
                  i32.gt_u
                  if  ;; label = @8
                    get_local 3
                    i32.load offset=24
                    set_local 9
                    block  ;; label = @9
                      get_local 3
                      i32.load offset=12
                      tee_local 1
                      get_local 3
                      i32.eq
                      if  ;; label = @10
                        get_local 3
                        i32.const 20
                        i32.add
                        tee_local 2
                        i32.load
                        tee_local 1
                        i32.eqz
                        if  ;; label = @11
                          get_local 3
                          i32.const 16
                          i32.add
                          tee_local 2
                          i32.load
                          tee_local 1
                          i32.eqz
                          if  ;; label = @12
                            i32.const 0
                            set_local 1
                            br 3 (;@9;)
                          end
                        end
                        loop  ;; label = @11
                          block  ;; label = @12
                            get_local 1
                            i32.const 20
                            i32.add
                            tee_local 5
                            i32.load
                            tee_local 8
                            i32.eqz
                            if  ;; label = @13
                              get_local 1
                              i32.const 16
                              i32.add
                              tee_local 5
                              i32.load
                              tee_local 8
                              i32.eqz
                              br_if 1 (;@12;)
                            end
                            get_local 5
                            set_local 2
                            get_local 8
                            set_local 1
                            br 1 (;@11;)
                          end
                        end
                        get_local 2
                        i32.const 0
                        i32.store
                      else
                        get_local 3
                        i32.load offset=8
                        tee_local 2
                        get_local 1
                        i32.store offset=12
                        get_local 1
                        get_local 2
                        i32.store offset=8
                      end
                    end
                    block  ;; label = @9
                      get_local 9
                      if  ;; label = @10
                        get_local 3
                        get_local 3
                        i32.load offset=28
                        tee_local 2
                        i32.const 2
                        i32.shl
                        i32.const 1332
                        i32.add
                        tee_local 5
                        i32.load
                        i32.eq
                        if  ;; label = @11
                          get_local 5
                          get_local 1
                          i32.store
                          get_local 1
                          i32.eqz
                          if  ;; label = @12
                            i32.const 1032
                            get_local 12
                            i32.const 1
                            get_local 2
                            i32.shl
                            i32.const -1
                            i32.xor
                            i32.and
                            i32.store
                            br 3 (;@9;)
                          end
                        else
                          get_local 9
                          i32.const 20
                          i32.add
                          set_local 2
                          get_local 9
                          i32.const 16
                          i32.add
                          tee_local 5
                          i32.load
                          get_local 3
                          i32.eq
                          if (result i32)  ;; label = @12
                            get_local 5
                          else
                            get_local 2
                          end
                          get_local 1
                          i32.store
                          get_local 1
                          i32.eqz
                          br_if 2 (;@9;)
                        end
                        get_local 1
                        get_local 9
                        i32.store offset=24
                        get_local 3
                        i32.load offset=16
                        tee_local 2
                        if  ;; label = @11
                          get_local 1
                          get_local 2
                          i32.store offset=16
                          get_local 2
                          get_local 1
                          i32.store offset=24
                        end
                        get_local 3
                        i32.load offset=20
                        tee_local 2
                        if  ;; label = @11
                          get_local 1
                          get_local 2
                          i32.store offset=20
                          get_local 2
                          get_local 1
                          i32.store offset=24
                        end
                      end
                    end
                    get_local 4
                    i32.const 16
                    i32.lt_u
                    if  ;; label = @9
                      get_local 3
                      get_local 4
                      get_local 0
                      i32.add
                      tee_local 0
                      i32.const 3
                      i32.or
                      i32.store offset=4
                      get_local 3
                      get_local 0
                      i32.add
                      i32.const 4
                      i32.add
                      tee_local 0
                      get_local 0
                      i32.load
                      i32.const 1
                      i32.or
                      i32.store
                    else
                      get_local 3
                      get_local 0
                      i32.const 3
                      i32.or
                      i32.store offset=4
                      get_local 11
                      get_local 4
                      i32.const 1
                      i32.or
                      i32.store offset=4
                      get_local 11
                      get_local 4
                      i32.add
                      get_local 4
                      i32.store
                      get_local 7
                      if  ;; label = @10
                        i32.const 1048
                        i32.load
                        set_local 5
                        get_local 7
                        i32.const 3
                        i32.shr_u
                        tee_local 1
                        i32.const 3
                        i32.shl
                        i32.const 1068
                        i32.add
                        set_local 0
                        i32.const 1
                        get_local 1
                        i32.shl
                        tee_local 1
                        get_local 6
                        i32.and
                        if (result i32)  ;; label = @11
                          get_local 0
                          i32.const 8
                          i32.add
                          tee_local 2
                          i32.load
                        else
                          i32.const 1028
                          get_local 1
                          get_local 6
                          i32.or
                          i32.store
                          get_local 0
                          i32.const 8
                          i32.add
                          set_local 2
                          get_local 0
                        end
                        set_local 1
                        get_local 2
                        get_local 5
                        i32.store
                        get_local 1
                        get_local 5
                        i32.store offset=12
                        get_local 5
                        get_local 1
                        i32.store offset=8
                        get_local 5
                        get_local 0
                        i32.store offset=12
                      end
                      i32.const 1036
                      get_local 4
                      i32.store
                      i32.const 1048
                      get_local 11
                      i32.store
                    end
                    get_local 10
                    set_global 4
                    get_local 3
                    i32.const 8
                    i32.add
                    return
                  end
                end
              end
            else
              get_local 0
              i32.const -65
              i32.gt_u
              if  ;; label = @6
                i32.const -1
                set_local 0
              else
                get_local 0
                i32.const 11
                i32.add
                tee_local 1
                i32.const -8
                i32.and
                set_local 0
                i32.const 1032
                i32.load
                tee_local 4
                if  ;; label = @7
                  get_local 1
                  i32.const 8
                  i32.shr_u
                  tee_local 1
                  if (result i32)  ;; label = @8
                    get_local 0
                    i32.const 16777215
                    i32.gt_u
                    if (result i32)  ;; label = @9
                      i32.const 31
                    else
                      get_local 0
                      i32.const 14
                      get_local 1
                      get_local 1
                      i32.const 1048320
                      i32.add
                      i32.const 16
                      i32.shr_u
                      i32.const 8
                      i32.and
                      tee_local 1
                      i32.shl
                      tee_local 2
                      i32.const 520192
                      i32.add
                      i32.const 16
                      i32.shr_u
                      i32.const 4
                      i32.and
                      tee_local 3
                      get_local 1
                      i32.or
                      get_local 2
                      get_local 3
                      i32.shl
                      tee_local 1
                      i32.const 245760
                      i32.add
                      i32.const 16
                      i32.shr_u
                      i32.const 2
                      i32.and
                      tee_local 2
                      i32.or
                      i32.sub
                      get_local 1
                      get_local 2
                      i32.shl
                      i32.const 15
                      i32.shr_u
                      i32.add
                      tee_local 1
                      i32.const 7
                      i32.add
                      i32.shr_u
                      i32.const 1
                      i32.and
                      get_local 1
                      i32.const 1
                      i32.shl
                      i32.or
                    end
                  else
                    i32.const 0
                  end
                  set_local 7
                  i32.const 0
                  get_local 0
                  i32.sub
                  set_local 3
                  block  ;; label = @8
                    block  ;; label = @9
                      get_local 7
                      i32.const 2
                      i32.shl
                      i32.const 1332
                      i32.add
                      i32.load
                      tee_local 1
                      if (result i32)  ;; label = @10
                        i32.const 25
                        get_local 7
                        i32.const 1
                        i32.shr_u
                        i32.sub
                        set_local 6
                        i32.const 0
                        set_local 2
                        get_local 0
                        get_local 7
                        i32.const 31
                        i32.eq
                        if (result i32)  ;; label = @11
                          i32.const 0
                        else
                          get_local 6
                        end
                        i32.shl
                        set_local 5
                        i32.const 0
                        set_local 6
                        loop  ;; label = @11
                          get_local 1
                          i32.load offset=4
                          i32.const -8
                          i32.and
                          get_local 0
                          i32.sub
                          tee_local 8
                          get_local 3
                          i32.lt_u
                          if  ;; label = @12
                            get_local 8
                            if (result i32)  ;; label = @13
                              get_local 8
                              set_local 3
                              get_local 1
                            else
                              i32.const 0
                              set_local 3
                              get_local 1
                              set_local 2
                              br 4 (;@9;)
                            end
                            set_local 2
                          end
                          get_local 1
                          i32.load offset=20
                          tee_local 8
                          i32.eqz
                          get_local 8
                          get_local 1
                          i32.const 16
                          i32.add
                          get_local 5
                          i32.const 31
                          i32.shr_u
                          i32.const 2
                          i32.shl
                          i32.add
                          i32.load
                          tee_local 1
                          i32.eq
                          i32.or
                          i32.eqz
                          if  ;; label = @12
                            get_local 8
                            set_local 6
                          end
                          get_local 5
                          i32.const 1
                          i32.shl
                          set_local 5
                          get_local 1
                          br_if 0 (;@11;)
                        end
                        get_local 2
                      else
                        i32.const 0
                      end
                      set_local 1
                      get_local 6
                      get_local 1
                      i32.or
                      if (result i32)  ;; label = @10
                        get_local 6
                      else
                        i32.const 2
                        get_local 7
                        i32.shl
                        tee_local 1
                        i32.const 0
                        get_local 1
                        i32.sub
                        i32.or
                        get_local 4
                        i32.and
                        tee_local 1
                        i32.eqz
                        br_if 6 (;@4;)
                        get_local 1
                        i32.const 0
                        get_local 1
                        i32.sub
                        i32.and
                        i32.const -1
                        i32.add
                        tee_local 6
                        i32.const 12
                        i32.shr_u
                        i32.const 16
                        i32.and
                        set_local 2
                        i32.const 0
                        set_local 1
                        get_local 6
                        get_local 2
                        i32.shr_u
                        tee_local 6
                        i32.const 5
                        i32.shr_u
                        i32.const 8
                        i32.and
                        tee_local 5
                        get_local 2
                        i32.or
                        get_local 6
                        get_local 5
                        i32.shr_u
                        tee_local 2
                        i32.const 2
                        i32.shr_u
                        i32.const 4
                        i32.and
                        tee_local 6
                        i32.or
                        get_local 2
                        get_local 6
                        i32.shr_u
                        tee_local 2
                        i32.const 1
                        i32.shr_u
                        i32.const 2
                        i32.and
                        tee_local 6
                        i32.or
                        get_local 2
                        get_local 6
                        i32.shr_u
                        tee_local 2
                        i32.const 1
                        i32.shr_u
                        i32.const 1
                        i32.and
                        tee_local 6
                        i32.or
                        get_local 2
                        get_local 6
                        i32.shr_u
                        i32.add
                        i32.const 2
                        i32.shl
                        i32.const 1332
                        i32.add
                        i32.load
                      end
                      tee_local 2
                      br_if 0 (;@9;)
                      get_local 1
                      set_local 6
                      br 1 (;@8;)
                    end
                    get_local 1
                    set_local 5
                    get_local 2
                    set_local 1
                    loop  ;; label = @9
                      get_local 1
                      i32.load offset=4
                      set_local 2
                      get_local 1
                      i32.load offset=16
                      tee_local 6
                      i32.eqz
                      if  ;; label = @10
                        get_local 1
                        i32.load offset=20
                        set_local 6
                      end
                      get_local 2
                      i32.const -8
                      i32.and
                      get_local 0
                      i32.sub
                      tee_local 2
                      get_local 3
                      i32.lt_u
                      tee_local 8
                      i32.eqz
                      if  ;; label = @10
                        get_local 3
                        set_local 2
                      end
                      get_local 8
                      i32.eqz
                      if  ;; label = @10
                        get_local 5
                        set_local 1
                      end
                      get_local 6
                      if (result i32)  ;; label = @10
                        get_local 1
                        set_local 5
                        get_local 2
                        set_local 3
                        get_local 6
                        set_local 1
                        br 1 (;@9;)
                      else
                        get_local 1
                        set_local 6
                        get_local 2
                      end
                      set_local 3
                    end
                  end
                  get_local 6
                  if  ;; label = @8
                    get_local 3
                    i32.const 1036
                    i32.load
                    get_local 0
                    i32.sub
                    i32.lt_u
                    if  ;; label = @9
                      get_local 6
                      get_local 0
                      i32.add
                      tee_local 7
                      get_local 6
                      i32.gt_u
                      if  ;; label = @10
                        get_local 6
                        i32.load offset=24
                        set_local 9
                        block  ;; label = @11
                          get_local 6
                          i32.load offset=12
                          tee_local 1
                          get_local 6
                          i32.eq
                          if  ;; label = @12
                            get_local 6
                            i32.const 20
                            i32.add
                            tee_local 2
                            i32.load
                            tee_local 1
                            i32.eqz
                            if  ;; label = @13
                              get_local 6
                              i32.const 16
                              i32.add
                              tee_local 2
                              i32.load
                              tee_local 1
                              i32.eqz
                              if  ;; label = @14
                                i32.const 0
                                set_local 1
                                br 3 (;@11;)
                              end
                            end
                            loop  ;; label = @13
                              block  ;; label = @14
                                get_local 1
                                i32.const 20
                                i32.add
                                tee_local 5
                                i32.load
                                tee_local 8
                                i32.eqz
                                if  ;; label = @15
                                  get_local 1
                                  i32.const 16
                                  i32.add
                                  tee_local 5
                                  i32.load
                                  tee_local 8
                                  i32.eqz
                                  br_if 1 (;@14;)
                                end
                                get_local 5
                                set_local 2
                                get_local 8
                                set_local 1
                                br 1 (;@13;)
                              end
                            end
                            get_local 2
                            i32.const 0
                            i32.store
                          else
                            get_local 6
                            i32.load offset=8
                            tee_local 2
                            get_local 1
                            i32.store offset=12
                            get_local 1
                            get_local 2
                            i32.store offset=8
                          end
                        end
                        block  ;; label = @11
                          get_local 9
                          if  ;; label = @12
                            get_local 6
                            get_local 6
                            i32.load offset=28
                            tee_local 2
                            i32.const 2
                            i32.shl
                            i32.const 1332
                            i32.add
                            tee_local 5
                            i32.load
                            i32.eq
                            if  ;; label = @13
                              get_local 5
                              get_local 1
                              i32.store
                              get_local 1
                              i32.eqz
                              if  ;; label = @14
                                i32.const 1032
                                get_local 4
                                i32.const 1
                                get_local 2
                                i32.shl
                                i32.const -1
                                i32.xor
                                i32.and
                                tee_local 1
                                i32.store
                                br 3 (;@11;)
                              end
                            else
                              get_local 9
                              i32.const 20
                              i32.add
                              set_local 2
                              get_local 9
                              i32.const 16
                              i32.add
                              tee_local 5
                              i32.load
                              get_local 6
                              i32.eq
                              if (result i32)  ;; label = @14
                                get_local 5
                              else
                                get_local 2
                              end
                              get_local 1
                              i32.store
                              get_local 1
                              i32.eqz
                              if  ;; label = @14
                                get_local 4
                                set_local 1
                                br 3 (;@11;)
                              end
                            end
                            get_local 1
                            get_local 9
                            i32.store offset=24
                            get_local 6
                            i32.load offset=16
                            tee_local 2
                            if  ;; label = @13
                              get_local 1
                              get_local 2
                              i32.store offset=16
                              get_local 2
                              get_local 1
                              i32.store offset=24
                            end
                            get_local 6
                            i32.load offset=20
                            tee_local 2
                            if  ;; label = @13
                              get_local 1
                              get_local 2
                              i32.store offset=20
                              get_local 2
                              get_local 1
                              i32.store offset=24
                            end
                          end
                          get_local 4
                          set_local 1
                        end
                        block  ;; label = @11
                          get_local 3
                          i32.const 16
                          i32.lt_u
                          if  ;; label = @12
                            get_local 6
                            get_local 3
                            get_local 0
                            i32.add
                            tee_local 0
                            i32.const 3
                            i32.or
                            i32.store offset=4
                            get_local 6
                            get_local 0
                            i32.add
                            i32.const 4
                            i32.add
                            tee_local 0
                            get_local 0
                            i32.load
                            i32.const 1
                            i32.or
                            i32.store
                          else
                            get_local 6
                            get_local 0
                            i32.const 3
                            i32.or
                            i32.store offset=4
                            get_local 7
                            get_local 3
                            i32.const 1
                            i32.or
                            i32.store offset=4
                            get_local 7
                            get_local 3
                            i32.add
                            get_local 3
                            i32.store
                            get_local 3
                            i32.const 3
                            i32.shr_u
                            set_local 2
                            get_local 3
                            i32.const 256
                            i32.lt_u
                            if  ;; label = @13
                              get_local 2
                              i32.const 3
                              i32.shl
                              i32.const 1068
                              i32.add
                              set_local 0
                              i32.const 1028
                              i32.load
                              tee_local 1
                              i32.const 1
                              get_local 2
                              i32.shl
                              tee_local 2
                              i32.and
                              if (result i32)  ;; label = @14
                                get_local 0
                                i32.const 8
                                i32.add
                                tee_local 2
                                i32.load
                              else
                                i32.const 1028
                                get_local 1
                                get_local 2
                                i32.or
                                i32.store
                                get_local 0
                                i32.const 8
                                i32.add
                                set_local 2
                                get_local 0
                              end
                              set_local 1
                              get_local 2
                              get_local 7
                              i32.store
                              get_local 1
                              get_local 7
                              i32.store offset=12
                              get_local 7
                              get_local 1
                              i32.store offset=8
                              get_local 7
                              get_local 0
                              i32.store offset=12
                              br 2 (;@11;)
                            end
                            get_local 3
                            i32.const 8
                            i32.shr_u
                            tee_local 0
                            if (result i32)  ;; label = @13
                              get_local 3
                              i32.const 16777215
                              i32.gt_u
                              if (result i32)  ;; label = @14
                                i32.const 31
                              else
                                get_local 3
                                i32.const 14
                                get_local 0
                                get_local 0
                                i32.const 1048320
                                i32.add
                                i32.const 16
                                i32.shr_u
                                i32.const 8
                                i32.and
                                tee_local 0
                                i32.shl
                                tee_local 2
                                i32.const 520192
                                i32.add
                                i32.const 16
                                i32.shr_u
                                i32.const 4
                                i32.and
                                tee_local 4
                                get_local 0
                                i32.or
                                get_local 2
                                get_local 4
                                i32.shl
                                tee_local 0
                                i32.const 245760
                                i32.add
                                i32.const 16
                                i32.shr_u
                                i32.const 2
                                i32.and
                                tee_local 2
                                i32.or
                                i32.sub
                                get_local 0
                                get_local 2
                                i32.shl
                                i32.const 15
                                i32.shr_u
                                i32.add
                                tee_local 0
                                i32.const 7
                                i32.add
                                i32.shr_u
                                i32.const 1
                                i32.and
                                get_local 0
                                i32.const 1
                                i32.shl
                                i32.or
                              end
                            else
                              i32.const 0
                            end
                            tee_local 2
                            i32.const 2
                            i32.shl
                            i32.const 1332
                            i32.add
                            set_local 0
                            get_local 7
                            get_local 2
                            i32.store offset=28
                            get_local 7
                            i32.const 16
                            i32.add
                            tee_local 4
                            i32.const 0
                            i32.store offset=4
                            get_local 4
                            i32.const 0
                            i32.store
                            get_local 1
                            i32.const 1
                            get_local 2
                            i32.shl
                            tee_local 4
                            i32.and
                            i32.eqz
                            if  ;; label = @13
                              i32.const 1032
                              get_local 1
                              get_local 4
                              i32.or
                              i32.store
                              get_local 0
                              get_local 7
                              i32.store
                              get_local 7
                              get_local 0
                              i32.store offset=24
                              get_local 7
                              get_local 7
                              i32.store offset=12
                              get_local 7
                              get_local 7
                              i32.store offset=8
                              br 2 (;@11;)
                            end
                            block  ;; label = @13
                              get_local 0
                              i32.load
                              tee_local 0
                              i32.load offset=4
                              i32.const -8
                              i32.and
                              get_local 3
                              i32.eq
                              if (result i32)  ;; label = @14
                                get_local 0
                              else
                                i32.const 25
                                get_local 2
                                i32.const 1
                                i32.shr_u
                                i32.sub
                                set_local 1
                                get_local 3
                                get_local 2
                                i32.const 31
                                i32.eq
                                if (result i32)  ;; label = @15
                                  i32.const 0
                                else
                                  get_local 1
                                end
                                i32.shl
                                set_local 2
                                loop  ;; label = @15
                                  get_local 0
                                  i32.const 16
                                  i32.add
                                  get_local 2
                                  i32.const 31
                                  i32.shr_u
                                  i32.const 2
                                  i32.shl
                                  i32.add
                                  tee_local 4
                                  i32.load
                                  tee_local 1
                                  if  ;; label = @16
                                    get_local 2
                                    i32.const 1
                                    i32.shl
                                    set_local 2
                                    get_local 1
                                    i32.load offset=4
                                    i32.const -8
                                    i32.and
                                    get_local 3
                                    i32.eq
                                    br_if 3 (;@13;)
                                    get_local 1
                                    set_local 0
                                    br 1 (;@15;)
                                  end
                                end
                                get_local 4
                                get_local 7
                                i32.store
                                get_local 7
                                get_local 0
                                i32.store offset=24
                                get_local 7
                                get_local 7
                                i32.store offset=12
                                get_local 7
                                get_local 7
                                i32.store offset=8
                                br 3 (;@11;)
                              end
                              set_local 1
                            end
                            get_local 1
                            i32.const 8
                            i32.add
                            tee_local 0
                            i32.load
                            tee_local 2
                            get_local 7
                            i32.store offset=12
                            get_local 0
                            get_local 7
                            i32.store
                            get_local 7
                            get_local 2
                            i32.store offset=8
                            get_local 7
                            get_local 1
                            i32.store offset=12
                            get_local 7
                            i32.const 0
                            i32.store offset=24
                          end
                        end
                        get_local 10
                        set_global 4
                        get_local 6
                        i32.const 8
                        i32.add
                        return
                      end
                    end
                  end
                end
              end
            end
          end
          i32.const 1036
          i32.load
          tee_local 2
          get_local 0
          i32.ge_u
          if  ;; label = @4
            i32.const 1048
            i32.load
            set_local 1
            get_local 2
            get_local 0
            i32.sub
            tee_local 3
            i32.const 15
            i32.gt_u
            if  ;; label = @5
              i32.const 1048
              get_local 1
              get_local 0
              i32.add
              tee_local 4
              i32.store
              i32.const 1036
              get_local 3
              i32.store
              get_local 4
              get_local 3
              i32.const 1
              i32.or
              i32.store offset=4
              get_local 1
              get_local 2
              i32.add
              get_local 3
              i32.store
              get_local 1
              get_local 0
              i32.const 3
              i32.or
              i32.store offset=4
            else
              i32.const 1036
              i32.const 0
              i32.store
              i32.const 1048
              i32.const 0
              i32.store
              get_local 1
              get_local 2
              i32.const 3
              i32.or
              i32.store offset=4
              get_local 1
              get_local 2
              i32.add
              i32.const 4
              i32.add
              tee_local 0
              get_local 0
              i32.load
              i32.const 1
              i32.or
              i32.store
            end
            br 2 (;@2;)
          end
          i32.const 1040
          i32.load
          tee_local 2
          get_local 0
          i32.gt_u
          if  ;; label = @4
            i32.const 1040
            get_local 2
            get_local 0
            i32.sub
            tee_local 2
            i32.store
            br 1 (;@3;)
          end
          i32.const 1500
          i32.load
          if (result i32)  ;; label = @4
            i32.const 1508
            i32.load
          else
            i32.const 1508
            i32.const 4096
            i32.store
            i32.const 1504
            i32.const 4096
            i32.store
            i32.const 1512
            i32.const -1
            i32.store
            i32.const 1516
            i32.const -1
            i32.store
            i32.const 1520
            i32.const 0
            i32.store
            i32.const 1472
            i32.const 0
            i32.store
            i32.const 1500
            get_local 10
            i32.const -16
            i32.and
            i32.const 1431655768
            i32.xor
            i32.store
            i32.const 4096
          end
          tee_local 1
          get_local 0
          i32.const 47
          i32.add
          tee_local 6
          i32.add
          tee_local 5
          i32.const 0
          get_local 1
          i32.sub
          tee_local 8
          i32.and
          tee_local 4
          get_local 0
          i32.le_u
          if  ;; label = @4
            br 3 (;@1;)
          end
          i32.const 1468
          i32.load
          tee_local 1
          if  ;; label = @4
            i32.const 1460
            i32.load
            tee_local 3
            get_local 4
            i32.add
            tee_local 7
            get_local 3
            i32.le_u
            get_local 7
            get_local 1
            i32.gt_u
            i32.or
            if  ;; label = @5
              br 4 (;@1;)
            end
          end
          get_local 0
          i32.const 48
          i32.add
          set_local 7
          block  ;; label = @4
            block  ;; label = @5
              i32.const 1472
              i32.load
              i32.const 4
              i32.and
              if  ;; label = @6
                i32.const 0
                set_local 2
              else
                block  ;; label = @7
                  block  ;; label = @8
                    block  ;; label = @9
                      i32.const 1052
                      i32.load
                      tee_local 1
                      i32.eqz
                      br_if 0 (;@9;)
                      i32.const 1476
                      set_local 3
                      loop  ;; label = @10
                        block  ;; label = @11
                          get_local 3
                          i32.load
                          tee_local 9
                          get_local 1
                          i32.le_u
                          if  ;; label = @12
                            get_local 9
                            get_local 3
                            i32.load offset=4
                            i32.add
                            get_local 1
                            i32.gt_u
                            br_if 1 (;@11;)
                          end
                          get_local 3
                          i32.load offset=8
                          tee_local 3
                          br_if 1 (;@10;)
                          br 2 (;@9;)
                        end
                      end
                      get_local 5
                      get_local 2
                      i32.sub
                      get_local 8
                      i32.and
                      tee_local 2
                      i32.const 2147483647
                      i32.lt_u
                      if  ;; label = @10
                        get_local 2
                        call 5
                        tee_local 1
                        get_local 3
                        i32.load
                        get_local 3
                        i32.load offset=4
                        i32.add
                        i32.eq
                        if  ;; label = @11
                          get_local 1
                          i32.const -1
                          i32.ne
                          br_if 6 (;@5;)
                        else
                          br 3 (;@8;)
                        end
                      else
                        i32.const 0
                        set_local 2
                      end
                      br 2 (;@7;)
                    end
                    i32.const 0
                    call 5
                    tee_local 1
                    i32.const -1
                    i32.eq
                    if (result i32)  ;; label = @9
                      i32.const 0
                    else
                      i32.const 1504
                      i32.load
                      tee_local 2
                      i32.const -1
                      i32.add
                      tee_local 3
                      get_local 1
                      i32.add
                      i32.const 0
                      get_local 2
                      i32.sub
                      i32.and
                      get_local 1
                      i32.sub
                      set_local 2
                      get_local 3
                      get_local 1
                      i32.and
                      if (result i32)  ;; label = @10
                        get_local 2
                      else
                        i32.const 0
                      end
                      get_local 4
                      i32.add
                      tee_local 2
                      i32.const 1460
                      i32.load
                      tee_local 5
                      i32.add
                      set_local 3
                      get_local 2
                      get_local 0
                      i32.gt_u
                      get_local 2
                      i32.const 2147483647
                      i32.lt_u
                      i32.and
                      if (result i32)  ;; label = @10
                        i32.const 1468
                        i32.load
                        tee_local 8
                        if  ;; label = @11
                          get_local 3
                          get_local 5
                          i32.le_u
                          get_local 3
                          get_local 8
                          i32.gt_u
                          i32.or
                          if  ;; label = @12
                            i32.const 0
                            set_local 2
                            br 5 (;@7;)
                          end
                        end
                        get_local 2
                        call 5
                        tee_local 3
                        get_local 1
                        i32.eq
                        br_if 5 (;@5;)
                        get_local 3
                        set_local 1
                        br 2 (;@8;)
                      else
                        i32.const 0
                      end
                    end
                    set_local 2
                    br 1 (;@7;)
                  end
                  get_local 7
                  get_local 2
                  i32.gt_u
                  get_local 2
                  i32.const 2147483647
                  i32.lt_u
                  get_local 1
                  i32.const -1
                  i32.ne
                  i32.and
                  i32.and
                  i32.eqz
                  if  ;; label = @8
                    get_local 1
                    i32.const -1
                    i32.eq
                    if  ;; label = @9
                      i32.const 0
                      set_local 2
                      br 2 (;@7;)
                    else
                      br 4 (;@5;)
                    end
                    unreachable
                  end
                  get_local 6
                  get_local 2
                  i32.sub
                  i32.const 1508
                  i32.load
                  tee_local 3
                  i32.add
                  i32.const 0
                  get_local 3
                  i32.sub
                  i32.and
                  tee_local 3
                  i32.const 2147483647
                  i32.ge_u
                  br_if 2 (;@5;)
                  i32.const 0
                  get_local 2
                  i32.sub
                  set_local 6
                  get_local 3
                  call 5
                  i32.const -1
                  i32.eq
                  if (result i32)  ;; label = @8
                    get_local 6
                    call 5
                    drop
                    i32.const 0
                  else
                    get_local 3
                    get_local 2
                    i32.add
                    set_local 2
                    br 3 (;@5;)
                  end
                  set_local 2
                end
                i32.const 1472
                i32.const 1472
                i32.load
                i32.const 4
                i32.or
                i32.store
              end
              get_local 4
              i32.const 2147483647
              i32.lt_u
              if  ;; label = @6
                get_local 4
                call 5
                tee_local 1
                i32.const 0
                call 5
                tee_local 3
                i32.lt_u
                get_local 1
                i32.const -1
                i32.ne
                get_local 3
                i32.const -1
                i32.ne
                i32.and
                i32.and
                set_local 4
                get_local 3
                get_local 1
                i32.sub
                tee_local 3
                get_local 0
                i32.const 40
                i32.add
                i32.gt_u
                tee_local 6
                if  ;; label = @7
                  get_local 3
                  set_local 2
                end
                get_local 1
                i32.const -1
                i32.eq
                get_local 6
                i32.const 1
                i32.xor
                i32.or
                get_local 4
                i32.const 1
                i32.xor
                i32.or
                i32.eqz
                br_if 1 (;@5;)
              end
              br 1 (;@4;)
            end
            i32.const 1460
            i32.const 1460
            i32.load
            get_local 2
            i32.add
            tee_local 3
            i32.store
            get_local 3
            i32.const 1464
            i32.load
            i32.gt_u
            if  ;; label = @5
              i32.const 1464
              get_local 3
              i32.store
            end
            block  ;; label = @5
              i32.const 1052
              i32.load
              tee_local 4
              if  ;; label = @6
                i32.const 1476
                set_local 3
                block  ;; label = @7
                  block  ;; label = @8
                    loop  ;; label = @9
                      get_local 1
                      get_local 3
                      i32.load
                      tee_local 6
                      get_local 3
                      i32.load offset=4
                      tee_local 5
                      i32.add
                      i32.eq
                      br_if 1 (;@8;)
                      get_local 3
                      i32.load offset=8
                      tee_local 3
                      br_if 0 (;@9;)
                    end
                    br 1 (;@7;)
                  end
                  get_local 3
                  i32.const 4
                  i32.add
                  set_local 8
                  get_local 3
                  i32.load offset=12
                  i32.const 8
                  i32.and
                  i32.eqz
                  if  ;; label = @8
                    get_local 1
                    get_local 4
                    i32.gt_u
                    get_local 6
                    get_local 4
                    i32.le_u
                    i32.and
                    if  ;; label = @9
                      get_local 8
                      get_local 5
                      get_local 2
                      i32.add
                      i32.store
                      i32.const 1040
                      i32.load
                      get_local 2
                      i32.add
                      set_local 2
                      i32.const 0
                      get_local 4
                      i32.const 8
                      i32.add
                      tee_local 3
                      i32.sub
                      i32.const 7
                      i32.and
                      set_local 1
                      i32.const 1052
                      get_local 4
                      get_local 3
                      i32.const 7
                      i32.and
                      if (result i32)  ;; label = @10
                        get_local 1
                      else
                        i32.const 0
                        tee_local 1
                      end
                      i32.add
                      tee_local 3
                      i32.store
                      i32.const 1040
                      get_local 2
                      get_local 1
                      i32.sub
                      tee_local 1
                      i32.store
                      get_local 3
                      get_local 1
                      i32.const 1
                      i32.or
                      i32.store offset=4
                      get_local 4
                      get_local 2
                      i32.add
                      i32.const 40
                      i32.store offset=4
                      i32.const 1056
                      i32.const 1516
                      i32.load
                      i32.store
                      br 4 (;@5;)
                    end
                  end
                end
                get_local 1
                i32.const 1044
                i32.load
                i32.lt_u
                if  ;; label = @7
                  i32.const 1044
                  get_local 1
                  i32.store
                end
                get_local 1
                get_local 2
                i32.add
                set_local 6
                i32.const 1476
                set_local 3
                block  ;; label = @7
                  block  ;; label = @8
                    loop  ;; label = @9
                      get_local 3
                      i32.load
                      get_local 6
                      i32.eq
                      br_if 1 (;@8;)
                      get_local 3
                      i32.load offset=8
                      tee_local 3
                      br_if 0 (;@9;)
                    end
                    br 1 (;@7;)
                  end
                  get_local 3
                  i32.load offset=12
                  i32.const 8
                  i32.and
                  i32.eqz
                  if  ;; label = @8
                    get_local 3
                    get_local 1
                    i32.store
                    get_local 3
                    i32.const 4
                    i32.add
                    tee_local 3
                    get_local 3
                    i32.load
                    get_local 2
                    i32.add
                    i32.store
                    i32.const 0
                    get_local 1
                    i32.const 8
                    i32.add
                    tee_local 2
                    i32.sub
                    i32.const 7
                    i32.and
                    set_local 3
                    i32.const 0
                    get_local 6
                    i32.const 8
                    i32.add
                    tee_local 8
                    i32.sub
                    i32.const 7
                    i32.and
                    set_local 9
                    get_local 1
                    get_local 2
                    i32.const 7
                    i32.and
                    if (result i32)  ;; label = @9
                      get_local 3
                    else
                      i32.const 0
                    end
                    i32.add
                    tee_local 7
                    get_local 0
                    i32.add
                    set_local 5
                    get_local 6
                    get_local 8
                    i32.const 7
                    i32.and
                    if (result i32)  ;; label = @9
                      get_local 9
                    else
                      i32.const 0
                    end
                    i32.add
                    tee_local 2
                    get_local 7
                    i32.sub
                    get_local 0
                    i32.sub
                    set_local 3
                    get_local 7
                    get_local 0
                    i32.const 3
                    i32.or
                    i32.store offset=4
                    block  ;; label = @9
                      get_local 4
                      get_local 2
                      i32.eq
                      if  ;; label = @10
                        i32.const 1040
                        i32.const 1040
                        i32.load
                        get_local 3
                        i32.add
                        tee_local 0
                        i32.store
                        i32.const 1052
                        get_local 5
                        i32.store
                        get_local 5
                        get_local 0
                        i32.const 1
                        i32.or
                        i32.store offset=4
                      else
                        i32.const 1048
                        i32.load
                        get_local 2
                        i32.eq
                        if  ;; label = @11
                          i32.const 1036
                          i32.const 1036
                          i32.load
                          get_local 3
                          i32.add
                          tee_local 0
                          i32.store
                          i32.const 1048
                          get_local 5
                          i32.store
                          get_local 5
                          get_local 0
                          i32.const 1
                          i32.or
                          i32.store offset=4
                          get_local 5
                          get_local 0
                          i32.add
                          get_local 0
                          i32.store
                          br 2 (;@9;)
                        end
                        get_local 2
                        i32.load offset=4
                        tee_local 0
                        i32.const 3
                        i32.and
                        i32.const 1
                        i32.eq
                        if  ;; label = @11
                          get_local 0
                          i32.const -8
                          i32.and
                          set_local 9
                          get_local 0
                          i32.const 3
                          i32.shr_u
                          set_local 4
                          block  ;; label = @12
                            get_local 0
                            i32.const 256
                            i32.lt_u
                            if  ;; label = @13
                              get_local 2
                              i32.load offset=12
                              tee_local 0
                              get_local 2
                              i32.load offset=8
                              tee_local 1
                              i32.eq
                              if  ;; label = @14
                                i32.const 1028
                                i32.const 1028
                                i32.load
                                i32.const 1
                                get_local 4
                                i32.shl
                                i32.const -1
                                i32.xor
                                i32.and
                                i32.store
                              else
                                get_local 1
                                get_local 0
                                i32.store offset=12
                                get_local 0
                                get_local 1
                                i32.store offset=8
                              end
                            else
                              get_local 2
                              i32.load offset=24
                              set_local 8
                              block  ;; label = @14
                                get_local 2
                                i32.load offset=12
                                tee_local 0
                                get_local 2
                                i32.eq
                                if  ;; label = @15
                                  get_local 2
                                  i32.const 16
                                  i32.add
                                  tee_local 1
                                  i32.const 4
                                  i32.add
                                  tee_local 4
                                  i32.load
                                  tee_local 0
                                  if  ;; label = @16
                                    get_local 4
                                    set_local 1
                                  else
                                    get_local 1
                                    i32.load
                                    tee_local 0
                                    i32.eqz
                                    if  ;; label = @17
                                      i32.const 0
                                      set_local 0
                                      br 3 (;@14;)
                                    end
                                  end
                                  loop  ;; label = @16
                                    block  ;; label = @17
                                      get_local 0
                                      i32.const 20
                                      i32.add
                                      tee_local 4
                                      i32.load
                                      tee_local 6
                                      i32.eqz
                                      if  ;; label = @18
                                        get_local 0
                                        i32.const 16
                                        i32.add
                                        tee_local 4
                                        i32.load
                                        tee_local 6
                                        i32.eqz
                                        br_if 1 (;@17;)
                                      end
                                      get_local 4
                                      set_local 1
                                      get_local 6
                                      set_local 0
                                      br 1 (;@16;)
                                    end
                                  end
                                  get_local 1
                                  i32.const 0
                                  i32.store
                                else
                                  get_local 2
                                  i32.load offset=8
                                  tee_local 1
                                  get_local 0
                                  i32.store offset=12
                                  get_local 0
                                  get_local 1
                                  i32.store offset=8
                                end
                              end
                              get_local 8
                              i32.eqz
                              br_if 1 (;@12;)
                              block  ;; label = @14
                                get_local 2
                                i32.load offset=28
                                tee_local 1
                                i32.const 2
                                i32.shl
                                i32.const 1332
                                i32.add
                                tee_local 4
                                i32.load
                                get_local 2
                                i32.eq
                                if  ;; label = @15
                                  get_local 4
                                  get_local 0
                                  i32.store
                                  get_local 0
                                  br_if 1 (;@14;)
                                  i32.const 1032
                                  i32.const 1032
                                  i32.load
                                  i32.const 1
                                  get_local 1
                                  i32.shl
                                  i32.const -1
                                  i32.xor
                                  i32.and
                                  i32.store
                                  br 3 (;@12;)
                                else
                                  get_local 8
                                  i32.const 20
                                  i32.add
                                  set_local 1
                                  get_local 8
                                  i32.const 16
                                  i32.add
                                  tee_local 4
                                  i32.load
                                  get_local 2
                                  i32.eq
                                  if (result i32)  ;; label = @16
                                    get_local 4
                                  else
                                    get_local 1
                                  end
                                  get_local 0
                                  i32.store
                                  get_local 0
                                  i32.eqz
                                  br_if 3 (;@12;)
                                end
                              end
                              get_local 0
                              get_local 8
                              i32.store offset=24
                              get_local 2
                              i32.const 16
                              i32.add
                              tee_local 4
                              i32.load
                              tee_local 1
                              if  ;; label = @14
                                get_local 0
                                get_local 1
                                i32.store offset=16
                                get_local 1
                                get_local 0
                                i32.store offset=24
                              end
                              get_local 4
                              i32.load offset=4
                              tee_local 1
                              i32.eqz
                              br_if 1 (;@12;)
                              get_local 0
                              get_local 1
                              i32.store offset=20
                              get_local 1
                              get_local 0
                              i32.store offset=24
                            end
                          end
                          get_local 2
                          get_local 9
                          i32.add
                          set_local 2
                          get_local 9
                          get_local 3
                          i32.add
                          set_local 3
                        end
                        get_local 2
                        i32.const 4
                        i32.add
                        tee_local 0
                        get_local 0
                        i32.load
                        i32.const -2
                        i32.and
                        i32.store
                        get_local 5
                        get_local 3
                        i32.const 1
                        i32.or
                        i32.store offset=4
                        get_local 5
                        get_local 3
                        i32.add
                        get_local 3
                        i32.store
                        get_local 3
                        i32.const 3
                        i32.shr_u
                        set_local 1
                        get_local 3
                        i32.const 256
                        i32.lt_u
                        if  ;; label = @11
                          get_local 1
                          i32.const 3
                          i32.shl
                          i32.const 1068
                          i32.add
                          set_local 0
                          i32.const 1028
                          i32.load
                          tee_local 2
                          i32.const 1
                          get_local 1
                          i32.shl
                          tee_local 1
                          i32.and
                          if (result i32)  ;; label = @12
                            get_local 0
                            i32.const 8
                            i32.add
                            tee_local 2
                            i32.load
                          else
                            i32.const 1028
                            get_local 2
                            get_local 1
                            i32.or
                            i32.store
                            get_local 0
                            i32.const 8
                            i32.add
                            set_local 2
                            get_local 0
                          end
                          set_local 1
                          get_local 2
                          get_local 5
                          i32.store
                          get_local 1
                          get_local 5
                          i32.store offset=12
                          get_local 5
                          get_local 1
                          i32.store offset=8
                          get_local 5
                          get_local 0
                          i32.store offset=12
                          br 2 (;@9;)
                        end
                        block (result i32)  ;; label = @11
                          get_local 3
                          i32.const 8
                          i32.shr_u
                          tee_local 0
                          if (result i32)  ;; label = @12
                            i32.const 31
                            get_local 3
                            i32.const 16777215
                            i32.gt_u
                            br_if 1 (;@11;)
                            drop
                            get_local 3
                            i32.const 14
                            get_local 0
                            get_local 0
                            i32.const 1048320
                            i32.add
                            i32.const 16
                            i32.shr_u
                            i32.const 8
                            i32.and
                            tee_local 0
                            i32.shl
                            tee_local 1
                            i32.const 520192
                            i32.add
                            i32.const 16
                            i32.shr_u
                            i32.const 4
                            i32.and
                            tee_local 2
                            get_local 0
                            i32.or
                            get_local 1
                            get_local 2
                            i32.shl
                            tee_local 0
                            i32.const 245760
                            i32.add
                            i32.const 16
                            i32.shr_u
                            i32.const 2
                            i32.and
                            tee_local 1
                            i32.or
                            i32.sub
                            get_local 0
                            get_local 1
                            i32.shl
                            i32.const 15
                            i32.shr_u
                            i32.add
                            tee_local 0
                            i32.const 7
                            i32.add
                            i32.shr_u
                            i32.const 1
                            i32.and
                            get_local 0
                            i32.const 1
                            i32.shl
                            i32.or
                          else
                            i32.const 0
                          end
                        end
                        tee_local 1
                        i32.const 2
                        i32.shl
                        i32.const 1332
                        i32.add
                        set_local 0
                        get_local 5
                        get_local 1
                        i32.store offset=28
                        get_local 5
                        i32.const 16
                        i32.add
                        tee_local 2
                        i32.const 0
                        i32.store offset=4
                        get_local 2
                        i32.const 0
                        i32.store
                        i32.const 1032
                        i32.load
                        tee_local 2
                        i32.const 1
                        get_local 1
                        i32.shl
                        tee_local 4
                        i32.and
                        i32.eqz
                        if  ;; label = @11
                          i32.const 1032
                          get_local 2
                          get_local 4
                          i32.or
                          i32.store
                          get_local 0
                          get_local 5
                          i32.store
                          get_local 5
                          get_local 0
                          i32.store offset=24
                          get_local 5
                          get_local 5
                          i32.store offset=12
                          get_local 5
                          get_local 5
                          i32.store offset=8
                          br 2 (;@9;)
                        end
                        block  ;; label = @11
                          get_local 0
                          i32.load
                          tee_local 0
                          i32.load offset=4
                          i32.const -8
                          i32.and
                          get_local 3
                          i32.eq
                          if (result i32)  ;; label = @12
                            get_local 0
                          else
                            i32.const 25
                            get_local 1
                            i32.const 1
                            i32.shr_u
                            i32.sub
                            set_local 2
                            get_local 3
                            get_local 1
                            i32.const 31
                            i32.eq
                            if (result i32)  ;; label = @13
                              i32.const 0
                            else
                              get_local 2
                            end
                            i32.shl
                            set_local 2
                            loop  ;; label = @13
                              get_local 0
                              i32.const 16
                              i32.add
                              get_local 2
                              i32.const 31
                              i32.shr_u
                              i32.const 2
                              i32.shl
                              i32.add
                              tee_local 4
                              i32.load
                              tee_local 1
                              if  ;; label = @14
                                get_local 2
                                i32.const 1
                                i32.shl
                                set_local 2
                                get_local 1
                                i32.load offset=4
                                i32.const -8
                                i32.and
                                get_local 3
                                i32.eq
                                br_if 3 (;@11;)
                                get_local 1
                                set_local 0
                                br 1 (;@13;)
                              end
                            end
                            get_local 4
                            get_local 5
                            i32.store
                            get_local 5
                            get_local 0
                            i32.store offset=24
                            get_local 5
                            get_local 5
                            i32.store offset=12
                            get_local 5
                            get_local 5
                            i32.store offset=8
                            br 3 (;@9;)
                          end
                          set_local 1
                        end
                        get_local 1
                        i32.const 8
                        i32.add
                        tee_local 0
                        i32.load
                        tee_local 2
                        get_local 5
                        i32.store offset=12
                        get_local 0
                        get_local 5
                        i32.store
                        get_local 5
                        get_local 2
                        i32.store offset=8
                        get_local 5
                        get_local 1
                        i32.store offset=12
                        get_local 5
                        i32.const 0
                        i32.store offset=24
                      end
                    end
                    get_local 10
                    set_global 4
                    get_local 7
                    i32.const 8
                    i32.add
                    return
                  end
                end
                i32.const 1476
                set_local 3
                loop  ;; label = @7
                  block  ;; label = @8
                    get_local 3
                    i32.load
                    tee_local 6
                    get_local 4
                    i32.le_u
                    if  ;; label = @9
                      get_local 6
                      get_local 3
                      i32.load offset=4
                      i32.add
                      tee_local 7
                      get_local 4
                      i32.gt_u
                      br_if 1 (;@8;)
                    end
                    get_local 3
                    i32.load offset=8
                    set_local 3
                    br 1 (;@7;)
                  end
                end
                i32.const 0
                get_local 7
                i32.const -47
                i32.add
                tee_local 3
                i32.const 8
                i32.add
                tee_local 6
                i32.sub
                i32.const 7
                i32.and
                set_local 5
                get_local 3
                get_local 6
                i32.const 7
                i32.and
                if (result i32)  ;; label = @7
                  get_local 5
                else
                  i32.const 0
                end
                i32.add
                tee_local 3
                get_local 4
                i32.const 16
                i32.add
                tee_local 12
                i32.lt_u
                if (result i32)  ;; label = @7
                  get_local 4
                  tee_local 3
                else
                  get_local 3
                end
                i32.const 8
                i32.add
                set_local 8
                get_local 3
                i32.const 24
                i32.add
                set_local 6
                get_local 2
                i32.const -40
                i32.add
                set_local 9
                i32.const 0
                get_local 1
                i32.const 8
                i32.add
                tee_local 11
                i32.sub
                i32.const 7
                i32.and
                set_local 5
                i32.const 1052
                get_local 1
                get_local 11
                i32.const 7
                i32.and
                if (result i32)  ;; label = @7
                  get_local 5
                else
                  i32.const 0
                  tee_local 5
                end
                i32.add
                tee_local 11
                i32.store
                i32.const 1040
                get_local 9
                get_local 5
                i32.sub
                tee_local 5
                i32.store
                get_local 11
                get_local 5
                i32.const 1
                i32.or
                i32.store offset=4
                get_local 1
                get_local 9
                i32.add
                i32.const 40
                i32.store offset=4
                i32.const 1056
                i32.const 1516
                i32.load
                i32.store
                get_local 3
                i32.const 4
                i32.add
                tee_local 5
                i32.const 27
                i32.store
                get_local 8
                i32.const 1476
                i64.load align=4
                i64.store align=4
                get_local 8
                i32.const 1484
                i64.load align=4
                i64.store offset=8 align=4
                i32.const 1476
                get_local 1
                i32.store
                i32.const 1480
                get_local 2
                i32.store
                i32.const 1488
                i32.const 0
                i32.store
                i32.const 1484
                get_local 8
                i32.store
                get_local 6
                set_local 1
                loop  ;; label = @7
                  get_local 1
                  i32.const 4
                  i32.add
                  tee_local 2
                  i32.const 7
                  i32.store
                  get_local 1
                  i32.const 8
                  i32.add
                  get_local 7
                  i32.lt_u
                  if  ;; label = @8
                    get_local 2
                    set_local 1
                    br 1 (;@7;)
                  end
                end
                get_local 3
                get_local 4
                i32.ne
                if  ;; label = @7
                  get_local 5
                  get_local 5
                  i32.load
                  i32.const -2
                  i32.and
                  i32.store
                  get_local 4
                  get_local 3
                  get_local 4
                  i32.sub
                  tee_local 6
                  i32.const 1
                  i32.or
                  i32.store offset=4
                  get_local 3
                  get_local 6
                  i32.store
                  get_local 6
                  i32.const 3
                  i32.shr_u
                  set_local 2
                  get_local 6
                  i32.const 256
                  i32.lt_u
                  if  ;; label = @8
                    get_local 2
                    i32.const 3
                    i32.shl
                    i32.const 1068
                    i32.add
                    set_local 1
                    i32.const 1028
                    i32.load
                    tee_local 3
                    i32.const 1
                    get_local 2
                    i32.shl
                    tee_local 2
                    i32.and
                    if (result i32)  ;; label = @9
                      get_local 1
                      i32.const 8
                      i32.add
                      tee_local 3
                      i32.load
                    else
                      i32.const 1028
                      get_local 3
                      get_local 2
                      i32.or
                      i32.store
                      get_local 1
                      i32.const 8
                      i32.add
                      set_local 3
                      get_local 1
                    end
                    set_local 2
                    get_local 3
                    get_local 4
                    i32.store
                    get_local 2
                    get_local 4
                    i32.store offset=12
                    get_local 4
                    get_local 2
                    i32.store offset=8
                    get_local 4
                    get_local 1
                    i32.store offset=12
                    br 3 (;@5;)
                  end
                  get_local 6
                  i32.const 8
                  i32.shr_u
                  tee_local 1
                  if (result i32)  ;; label = @8
                    get_local 6
                    i32.const 16777215
                    i32.gt_u
                    if (result i32)  ;; label = @9
                      i32.const 31
                    else
                      get_local 6
                      i32.const 14
                      get_local 1
                      get_local 1
                      i32.const 1048320
                      i32.add
                      i32.const 16
                      i32.shr_u
                      i32.const 8
                      i32.and
                      tee_local 1
                      i32.shl
                      tee_local 2
                      i32.const 520192
                      i32.add
                      i32.const 16
                      i32.shr_u
                      i32.const 4
                      i32.and
                      tee_local 3
                      get_local 1
                      i32.or
                      get_local 2
                      get_local 3
                      i32.shl
                      tee_local 1
                      i32.const 245760
                      i32.add
                      i32.const 16
                      i32.shr_u
                      i32.const 2
                      i32.and
                      tee_local 2
                      i32.or
                      i32.sub
                      get_local 1
                      get_local 2
                      i32.shl
                      i32.const 15
                      i32.shr_u
                      i32.add
                      tee_local 1
                      i32.const 7
                      i32.add
                      i32.shr_u
                      i32.const 1
                      i32.and
                      get_local 1
                      i32.const 1
                      i32.shl
                      i32.or
                    end
                  else
                    i32.const 0
                  end
                  tee_local 2
                  i32.const 2
                  i32.shl
                  i32.const 1332
                  i32.add
                  set_local 1
                  get_local 4
                  get_local 2
                  i32.store offset=28
                  get_local 4
                  i32.const 0
                  i32.store offset=20
                  get_local 12
                  i32.const 0
                  i32.store
                  i32.const 1032
                  i32.load
                  tee_local 3
                  i32.const 1
                  get_local 2
                  i32.shl
                  tee_local 5
                  i32.and
                  i32.eqz
                  if  ;; label = @8
                    i32.const 1032
                    get_local 3
                    get_local 5
                    i32.or
                    i32.store
                    get_local 1
                    get_local 4
                    i32.store
                    get_local 4
                    get_local 1
                    i32.store offset=24
                    get_local 4
                    get_local 4
                    i32.store offset=12
                    get_local 4
                    get_local 4
                    i32.store offset=8
                    br 3 (;@5;)
                  end
                  block  ;; label = @8
                    get_local 1
                    i32.load
                    tee_local 1
                    i32.load offset=4
                    i32.const -8
                    i32.and
                    get_local 6
                    i32.eq
                    if (result i32)  ;; label = @9
                      get_local 1
                    else
                      i32.const 25
                      get_local 2
                      i32.const 1
                      i32.shr_u
                      i32.sub
                      set_local 3
                      get_local 6
                      get_local 2
                      i32.const 31
                      i32.eq
                      if (result i32)  ;; label = @10
                        i32.const 0
                      else
                        get_local 3
                      end
                      i32.shl
                      set_local 3
                      loop  ;; label = @10
                        get_local 1
                        i32.const 16
                        i32.add
                        get_local 3
                        i32.const 31
                        i32.shr_u
                        i32.const 2
                        i32.shl
                        i32.add
                        tee_local 5
                        i32.load
                        tee_local 2
                        if  ;; label = @11
                          get_local 3
                          i32.const 1
                          i32.shl
                          set_local 3
                          get_local 2
                          i32.load offset=4
                          i32.const -8
                          i32.and
                          get_local 6
                          i32.eq
                          br_if 3 (;@8;)
                          get_local 2
                          set_local 1
                          br 1 (;@10;)
                        end
                      end
                      get_local 5
                      get_local 4
                      i32.store
                      get_local 4
                      get_local 1
                      i32.store offset=24
                      get_local 4
                      get_local 4
                      i32.store offset=12
                      get_local 4
                      get_local 4
                      i32.store offset=8
                      br 4 (;@5;)
                    end
                    set_local 2
                  end
                  get_local 2
                  i32.const 8
                  i32.add
                  tee_local 1
                  i32.load
                  tee_local 3
                  get_local 4
                  i32.store offset=12
                  get_local 1
                  get_local 4
                  i32.store
                  get_local 4
                  get_local 3
                  i32.store offset=8
                  get_local 4
                  get_local 2
                  i32.store offset=12
                  get_local 4
                  i32.const 0
                  i32.store offset=24
                end
              else
                i32.const 1044
                i32.load
                tee_local 3
                i32.eqz
                get_local 1
                get_local 3
                i32.lt_u
                i32.or
                if  ;; label = @7
                  i32.const 1044
                  get_local 1
                  i32.store
                end
                i32.const 1476
                get_local 1
                i32.store
                i32.const 1480
                get_local 2
                i32.store
                i32.const 1488
                i32.const 0
                i32.store
                i32.const 1064
                i32.const 1500
                i32.load
                i32.store
                i32.const 1060
                i32.const -1
                i32.store
                i32.const 1080
                i32.const 1068
                i32.store
                i32.const 1076
                i32.const 1068
                i32.store
                i32.const 1088
                i32.const 1076
                i32.store
                i32.const 1084
                i32.const 1076
                i32.store
                i32.const 1096
                i32.const 1084
                i32.store
                i32.const 1092
                i32.const 1084
                i32.store
                i32.const 1104
                i32.const 1092
                i32.store
                i32.const 1100
                i32.const 1092
                i32.store
                i32.const 1112
                i32.const 1100
                i32.store
                i32.const 1108
                i32.const 1100
                i32.store
                i32.const 1120
                i32.const 1108
                i32.store
                i32.const 1116
                i32.const 1108
                i32.store
                i32.const 1128
                i32.const 1116
                i32.store
                i32.const 1124
                i32.const 1116
                i32.store
                i32.const 1136
                i32.const 1124
                i32.store
                i32.const 1132
                i32.const 1124
                i32.store
                i32.const 1144
                i32.const 1132
                i32.store
                i32.const 1140
                i32.const 1132
                i32.store
                i32.const 1152
                i32.const 1140
                i32.store
                i32.const 1148
                i32.const 1140
                i32.store
                i32.const 1160
                i32.const 1148
                i32.store
                i32.const 1156
                i32.const 1148
                i32.store
                i32.const 1168
                i32.const 1156
                i32.store
                i32.const 1164
                i32.const 1156
                i32.store
                i32.const 1176
                i32.const 1164
                i32.store
                i32.const 1172
                i32.const 1164
                i32.store
                i32.const 1184
                i32.const 1172
                i32.store
                i32.const 1180
                i32.const 1172
                i32.store
                i32.const 1192
                i32.const 1180
                i32.store
                i32.const 1188
                i32.const 1180
                i32.store
                i32.const 1200
                i32.const 1188
                i32.store
                i32.const 1196
                i32.const 1188
                i32.store
                i32.const 1208
                i32.const 1196
                i32.store
                i32.const 1204
                i32.const 1196
                i32.store
                i32.const 1216
                i32.const 1204
                i32.store
                i32.const 1212
                i32.const 1204
                i32.store
                i32.const 1224
                i32.const 1212
                i32.store
                i32.const 1220
                i32.const 1212
                i32.store
                i32.const 1232
                i32.const 1220
                i32.store
                i32.const 1228
                i32.const 1220
                i32.store
                i32.const 1240
                i32.const 1228
                i32.store
                i32.const 1236
                i32.const 1228
                i32.store
                i32.const 1248
                i32.const 1236
                i32.store
                i32.const 1244
                i32.const 1236
                i32.store
                i32.const 1256
                i32.const 1244
                i32.store
                i32.const 1252
                i32.const 1244
                i32.store
                i32.const 1264
                i32.const 1252
                i32.store
                i32.const 1260
                i32.const 1252
                i32.store
                i32.const 1272
                i32.const 1260
                i32.store
                i32.const 1268
                i32.const 1260
                i32.store
                i32.const 1280
                i32.const 1268
                i32.store
                i32.const 1276
                i32.const 1268
                i32.store
                i32.const 1288
                i32.const 1276
                i32.store
                i32.const 1284
                i32.const 1276
                i32.store
                i32.const 1296
                i32.const 1284
                i32.store
                i32.const 1292
                i32.const 1284
                i32.store
                i32.const 1304
                i32.const 1292
                i32.store
                i32.const 1300
                i32.const 1292
                i32.store
                i32.const 1312
                i32.const 1300
                i32.store
                i32.const 1308
                i32.const 1300
                i32.store
                i32.const 1320
                i32.const 1308
                i32.store
                i32.const 1316
                i32.const 1308
                i32.store
                i32.const 1328
                i32.const 1316
                i32.store
                i32.const 1324
                i32.const 1316
                i32.store
                get_local 2
                i32.const -40
                i32.add
                set_local 3
                i32.const 0
                get_local 1
                i32.const 8
                i32.add
                tee_local 4
                i32.sub
                i32.const 7
                i32.and
                set_local 2
                i32.const 1052
                get_local 1
                get_local 4
                i32.const 7
                i32.and
                if (result i32)  ;; label = @7
                  get_local 2
                else
                  i32.const 0
                  tee_local 2
                end
                i32.add
                tee_local 4
                i32.store
                i32.const 1040
                get_local 3
                get_local 2
                i32.sub
                tee_local 2
                i32.store
                get_local 4
                get_local 2
                i32.const 1
                i32.or
                i32.store offset=4
                get_local 1
                get_local 3
                i32.add
                i32.const 40
                i32.store offset=4
                i32.const 1056
                i32.const 1516
                i32.load
                i32.store
              end
            end
            i32.const 1040
            i32.load
            tee_local 1
            get_local 0
            i32.gt_u
            if  ;; label = @5
              i32.const 1040
              get_local 1
              get_local 0
              i32.sub
              tee_local 2
              i32.store
              br 2 (;@3;)
            end
          end
          i32.const 1524
          i32.const 12
          i32.store
          br 2 (;@1;)
        end
        i32.const 1052
        i32.const 1052
        i32.load
        tee_local 1
        get_local 0
        i32.add
        tee_local 3
        i32.store
        get_local 3
        get_local 2
        i32.const 1
        i32.or
        i32.store offset=4
        get_local 1
        get_local 0
        i32.const 3
        i32.or
        i32.store offset=4
      end
      get_local 10
      set_global 4
      get_local 1
      i32.const 8
      i32.add
      return
    end
    get_local 10
    set_global 4
    i32.const 0)
  (func (;7;) (type 3) (param i32 i32)
    (local i32 i32 i32 i32)
    get_local 0
    i32.const 1
    i32.add
    set_local 2
    get_local 0
    i32.load8_s
    i32.const 35
    i32.ne
    if  ;; label = @1
      get_local 0
      set_local 2
    end
    get_local 1
    f32.const 0x1p+0 (;=1;)
    f32.store offset=12
    get_local 1
    i32.const 8
    i32.add
    tee_local 3
    f32.const 0x1p+0 (;=1;)
    f32.store
    get_local 1
    i32.const 4
    i32.add
    tee_local 4
    f32.const 0x1p+0 (;=1;)
    f32.store
    get_local 1
    f32.const 0x1p+0 (;=1;)
    f32.store
    get_local 2
    call 10
    i32.const 6
    i32.ne
    if  ;; label = @1
      return
    end
    get_local 2
    i32.load8_s offset=1
    set_local 5
    get_local 2
    i32.load8_s
    i32.const 1024
    i32.load
    i32.const 1
    i32.and
    call_indirect (type 0)
    i32.const 4
    i32.shl
    set_local 0
    get_local 1
    get_local 5
    i32.const 1024
    i32.load
    i32.const 1
    i32.and
    call_indirect (type 0)
    get_local 0
    i32.add
    f64.convert_s/i32
    f64.const 0x1.fep+7 (;=255;)
    f64.div
    f32.demote/f64
    f32.store
    get_local 2
    i32.load8_s offset=3
    set_local 1
    get_local 2
    i32.load8_s offset=2
    i32.const 1024
    i32.load
    i32.const 1
    i32.and
    call_indirect (type 0)
    i32.const 4
    i32.shl
    set_local 0
    get_local 4
    get_local 1
    i32.const 1024
    i32.load
    i32.const 1
    i32.and
    call_indirect (type 0)
    get_local 0
    i32.add
    f64.convert_s/i32
    f64.const 0x1.fep+7 (;=255;)
    f64.div
    f32.demote/f64
    f32.store
    get_local 2
    i32.load8_s offset=5
    set_local 1
    get_local 2
    i32.load8_s offset=4
    i32.const 1024
    i32.load
    i32.const 1
    i32.and
    call_indirect (type 0)
    i32.const 4
    i32.shl
    set_local 0
    get_local 3
    get_local 1
    i32.const 1024
    i32.load
    i32.const 1
    i32.and
    call_indirect (type 0)
    get_local 0
    i32.add
    f64.convert_s/i32
    f64.const 0x1.fep+7 (;=255;)
    f64.div
    f32.demote/f64
    f32.store)
  (func (;8;) (type 0) (param i32) (result i32)
    (local i32)
    get_local 0
    i32.const 24
    i32.shl
    i32.const 24
    i32.shr_s
    set_local 1
    get_local 0
    i32.const -48
    i32.add
    i32.const 24
    i32.shl
    i32.const 24
    i32.shr_s
    i32.const 255
    i32.and
    i32.const 10
    i32.lt_s
    if (result i32)  ;; label = @1
      get_local 1
      i32.const -48
      i32.add
    else
      get_local 1
      i32.const 32
      i32.or
      set_local 0
      get_local 1
      i32.const -65
      i32.add
      i32.const 26
      i32.lt_u
      if (result i32)  ;; label = @2
        get_local 0
      else
        get_local 1
      end
      i32.const 24
      i32.shl
      tee_local 0
      i32.const 24
      i32.shr_s
      i32.const -87
      i32.add
      set_local 1
      get_local 0
      i32.const -1610612737
      i32.add
      i32.const 117440511
      i32.lt_u
      if (result i32)  ;; label = @2
        get_local 1
      else
        i32.const -1
      end
    end)
  (func (;9;) (type 0) (param i32) (result i32)
    i32.const 0
    call 0
    i32.const 0)
  (func (;10;) (type 0) (param i32) (result i32)
    (local i32 i32 i32)
    block  ;; label = @1
      get_local 0
      tee_local 2
      i32.const 3
      i32.and
      if  ;; label = @2
        get_local 2
        tee_local 1
        set_local 0
        loop  ;; label = @3
          get_local 1
          i32.load8_s
          i32.eqz
          br_if 2 (;@1;)
          get_local 1
          i32.const 1
          i32.add
          tee_local 1
          tee_local 0
          i32.const 3
          i32.and
          br_if 0 (;@3;)
        end
        get_local 1
        set_local 0
      end
      loop  ;; label = @2
        get_local 0
        i32.const 4
        i32.add
        set_local 1
        get_local 0
        i32.load
        tee_local 3
        i32.const -2139062144
        i32.and
        i32.const -2139062144
        i32.xor
        get_local 3
        i32.const -16843009
        i32.add
        i32.and
        i32.eqz
        if  ;; label = @3
          get_local 1
          set_local 0
          br 1 (;@2;)
        end
      end
      get_local 3
      i32.const 255
      i32.and
      if  ;; label = @2
        loop  ;; label = @3
          get_local 0
          i32.const 1
          i32.add
          tee_local 0
          i32.load8_s
          br_if 0 (;@3;)
        end
      end
    end
    get_local 0
    get_local 2
    i32.sub)
  (func (;11;) (type 2) (result i32)
    i32.const 1524)
  (func (;12;) (type 1) (param i32)
    (local i32 i32 i32 i32 i32 i32 i32 i32)
    get_local 0
    i32.eqz
    if  ;; label = @1
      return
    end
    i32.const 1044
    i32.load
    set_local 4
    get_local 0
    i32.const -8
    i32.add
    tee_local 2
    get_local 0
    i32.const -4
    i32.add
    i32.load
    tee_local 3
    i32.const -8
    i32.and
    tee_local 0
    i32.add
    set_local 5
    block (result i32)  ;; label = @1
      get_local 3
      i32.const 1
      i32.and
      if (result i32)  ;; label = @2
        get_local 2
      else
        get_local 2
        i32.load
        set_local 1
        get_local 3
        i32.const 3
        i32.and
        i32.eqz
        if  ;; label = @3
          return
        end
        get_local 2
        get_local 1
        i32.sub
        tee_local 2
        get_local 4
        i32.lt_u
        if  ;; label = @3
          return
        end
        get_local 1
        get_local 0
        i32.add
        set_local 0
        i32.const 1048
        i32.load
        get_local 2
        i32.eq
        if  ;; label = @3
          get_local 2
          get_local 5
          i32.const 4
          i32.add
          tee_local 1
          i32.load
          tee_local 3
          i32.const 3
          i32.and
          i32.const 3
          i32.ne
          br_if 2 (;@1;)
          drop
          i32.const 1036
          get_local 0
          i32.store
          get_local 1
          get_local 3
          i32.const -2
          i32.and
          i32.store
          get_local 2
          get_local 0
          i32.const 1
          i32.or
          i32.store offset=4
          get_local 2
          get_local 0
          i32.add
          get_local 0
          i32.store
          return
        end
        get_local 1
        i32.const 3
        i32.shr_u
        set_local 4
        get_local 1
        i32.const 256
        i32.lt_u
        if  ;; label = @3
          get_local 2
          i32.load offset=12
          tee_local 1
          get_local 2
          i32.load offset=8
          tee_local 3
          i32.eq
          if  ;; label = @4
            i32.const 1028
            i32.const 1028
            i32.load
            i32.const 1
            get_local 4
            i32.shl
            i32.const -1
            i32.xor
            i32.and
            i32.store
          else
            get_local 3
            get_local 1
            i32.store offset=12
            get_local 1
            get_local 3
            i32.store offset=8
          end
          get_local 2
          br 2 (;@1;)
        end
        get_local 2
        i32.load offset=24
        set_local 7
        block  ;; label = @3
          get_local 2
          i32.load offset=12
          tee_local 1
          get_local 2
          i32.eq
          if  ;; label = @4
            get_local 2
            i32.const 16
            i32.add
            tee_local 3
            i32.const 4
            i32.add
            tee_local 4
            i32.load
            tee_local 1
            if  ;; label = @5
              get_local 4
              set_local 3
            else
              get_local 3
              i32.load
              tee_local 1
              i32.eqz
              if  ;; label = @6
                i32.const 0
                set_local 1
                br 3 (;@3;)
              end
            end
            loop  ;; label = @5
              block  ;; label = @6
                get_local 1
                i32.const 20
                i32.add
                tee_local 4
                i32.load
                tee_local 6
                i32.eqz
                if  ;; label = @7
                  get_local 1
                  i32.const 16
                  i32.add
                  tee_local 4
                  i32.load
                  tee_local 6
                  i32.eqz
                  br_if 1 (;@6;)
                end
                get_local 4
                set_local 3
                get_local 6
                set_local 1
                br 1 (;@5;)
              end
            end
            get_local 3
            i32.const 0
            i32.store
          else
            get_local 2
            i32.load offset=8
            tee_local 3
            get_local 1
            i32.store offset=12
            get_local 1
            get_local 3
            i32.store offset=8
          end
        end
        get_local 7
        if (result i32)  ;; label = @3
          get_local 2
          i32.load offset=28
          tee_local 3
          i32.const 2
          i32.shl
          i32.const 1332
          i32.add
          tee_local 4
          i32.load
          get_local 2
          i32.eq
          if  ;; label = @4
            get_local 4
            get_local 1
            i32.store
            get_local 1
            i32.eqz
            if  ;; label = @5
              i32.const 1032
              i32.const 1032
              i32.load
              i32.const 1
              get_local 3
              i32.shl
              i32.const -1
              i32.xor
              i32.and
              i32.store
              get_local 2
              br 4 (;@1;)
            end
          else
            get_local 7
            i32.const 20
            i32.add
            set_local 3
            get_local 7
            i32.const 16
            i32.add
            tee_local 4
            i32.load
            get_local 2
            i32.eq
            if (result i32)  ;; label = @5
              get_local 4
            else
              get_local 3
            end
            get_local 1
            i32.store
            get_local 2
            get_local 1
            i32.eqz
            br_if 3 (;@1;)
            drop
          end
          get_local 1
          get_local 7
          i32.store offset=24
          get_local 2
          i32.const 16
          i32.add
          tee_local 4
          i32.load
          tee_local 3
          if  ;; label = @4
            get_local 1
            get_local 3
            i32.store offset=16
            get_local 3
            get_local 1
            i32.store offset=24
          end
          get_local 4
          i32.load offset=4
          tee_local 3
          if  ;; label = @4
            get_local 1
            get_local 3
            i32.store offset=20
            get_local 3
            get_local 1
            i32.store offset=24
          end
          get_local 2
        else
          get_local 2
        end
      end
    end
    tee_local 7
    get_local 5
    i32.ge_u
    if  ;; label = @1
      return
    end
    get_local 5
    i32.const 4
    i32.add
    tee_local 3
    i32.load
    tee_local 1
    i32.const 1
    i32.and
    i32.eqz
    if  ;; label = @1
      return
    end
    get_local 1
    i32.const 2
    i32.and
    if  ;; label = @1
      get_local 3
      get_local 1
      i32.const -2
      i32.and
      i32.store
      get_local 2
      get_local 0
      i32.const 1
      i32.or
      i32.store offset=4
      get_local 7
      get_local 0
      i32.add
      get_local 0
      i32.store
      get_local 0
      set_local 3
    else
      i32.const 1052
      i32.load
      get_local 5
      i32.eq
      if  ;; label = @2
        i32.const 1040
        i32.const 1040
        i32.load
        get_local 0
        i32.add
        tee_local 0
        i32.store
        i32.const 1052
        get_local 2
        i32.store
        get_local 2
        get_local 0
        i32.const 1
        i32.or
        i32.store offset=4
        get_local 2
        i32.const 1048
        i32.load
        i32.ne
        if  ;; label = @3
          return
        end
        i32.const 1048
        i32.const 0
        i32.store
        i32.const 1036
        i32.const 0
        i32.store
        return
      end
      i32.const 1048
      i32.load
      get_local 5
      i32.eq
      if  ;; label = @2
        i32.const 1036
        i32.const 1036
        i32.load
        get_local 0
        i32.add
        tee_local 0
        i32.store
        i32.const 1048
        get_local 7
        i32.store
        get_local 2
        get_local 0
        i32.const 1
        i32.or
        i32.store offset=4
        get_local 7
        get_local 0
        i32.add
        get_local 0
        i32.store
        return
      end
      get_local 1
      i32.const -8
      i32.and
      get_local 0
      i32.add
      set_local 3
      get_local 1
      i32.const 3
      i32.shr_u
      set_local 4
      block  ;; label = @2
        get_local 1
        i32.const 256
        i32.lt_u
        if  ;; label = @3
          get_local 5
          i32.load offset=12
          tee_local 0
          get_local 5
          i32.load offset=8
          tee_local 1
          i32.eq
          if  ;; label = @4
            i32.const 1028
            i32.const 1028
            i32.load
            i32.const 1
            get_local 4
            i32.shl
            i32.const -1
            i32.xor
            i32.and
            i32.store
          else
            get_local 1
            get_local 0
            i32.store offset=12
            get_local 0
            get_local 1
            i32.store offset=8
          end
        else
          get_local 5
          i32.load offset=24
          set_local 8
          block  ;; label = @4
            get_local 5
            i32.load offset=12
            tee_local 0
            get_local 5
            i32.eq
            if  ;; label = @5
              get_local 5
              i32.const 16
              i32.add
              tee_local 1
              i32.const 4
              i32.add
              tee_local 4
              i32.load
              tee_local 0
              if  ;; label = @6
                get_local 4
                set_local 1
              else
                get_local 1
                i32.load
                tee_local 0
                i32.eqz
                if  ;; label = @7
                  i32.const 0
                  set_local 0
                  br 3 (;@4;)
                end
              end
              loop  ;; label = @6
                block  ;; label = @7
                  get_local 0
                  i32.const 20
                  i32.add
                  tee_local 4
                  i32.load
                  tee_local 6
                  i32.eqz
                  if  ;; label = @8
                    get_local 0
                    i32.const 16
                    i32.add
                    tee_local 4
                    i32.load
                    tee_local 6
                    i32.eqz
                    br_if 1 (;@7;)
                  end
                  get_local 4
                  set_local 1
                  get_local 6
                  set_local 0
                  br 1 (;@6;)
                end
              end
              get_local 1
              i32.const 0
              i32.store
            else
              get_local 5
              i32.load offset=8
              tee_local 1
              get_local 0
              i32.store offset=12
              get_local 0
              get_local 1
              i32.store offset=8
            end
          end
          get_local 8
          if  ;; label = @4
            get_local 5
            i32.load offset=28
            tee_local 1
            i32.const 2
            i32.shl
            i32.const 1332
            i32.add
            tee_local 4
            i32.load
            get_local 5
            i32.eq
            if  ;; label = @5
              get_local 4
              get_local 0
              i32.store
              get_local 0
              i32.eqz
              if  ;; label = @6
                i32.const 1032
                i32.const 1032
                i32.load
                i32.const 1
                get_local 1
                i32.shl
                i32.const -1
                i32.xor
                i32.and
                i32.store
                br 4 (;@2;)
              end
            else
              get_local 8
              i32.const 20
              i32.add
              set_local 1
              get_local 8
              i32.const 16
              i32.add
              tee_local 4
              i32.load
              get_local 5
              i32.eq
              if (result i32)  ;; label = @6
                get_local 4
              else
                get_local 1
              end
              get_local 0
              i32.store
              get_local 0
              i32.eqz
              br_if 3 (;@2;)
            end
            get_local 0
            get_local 8
            i32.store offset=24
            get_local 5
            i32.const 16
            i32.add
            tee_local 4
            i32.load
            tee_local 1
            if  ;; label = @5
              get_local 0
              get_local 1
              i32.store offset=16
              get_local 1
              get_local 0
              i32.store offset=24
            end
            get_local 4
            i32.load offset=4
            tee_local 1
            if  ;; label = @5
              get_local 0
              get_local 1
              i32.store offset=20
              get_local 1
              get_local 0
              i32.store offset=24
            end
          end
        end
      end
      get_local 2
      get_local 3
      i32.const 1
      i32.or
      i32.store offset=4
      get_local 7
      get_local 3
      i32.add
      get_local 3
      i32.store
      get_local 2
      i32.const 1048
      i32.load
      i32.eq
      if  ;; label = @2
        i32.const 1036
        get_local 3
        i32.store
        return
      end
    end
    get_local 3
    i32.const 3
    i32.shr_u
    set_local 1
    get_local 3
    i32.const 256
    i32.lt_u
    if  ;; label = @1
      get_local 1
      i32.const 3
      i32.shl
      i32.const 1068
      i32.add
      set_local 0
      i32.const 1028
      i32.load
      tee_local 3
      i32.const 1
      get_local 1
      i32.shl
      tee_local 1
      i32.and
      if (result i32)  ;; label = @2
        get_local 0
        i32.const 8
        i32.add
        tee_local 3
        i32.load
      else
        i32.const 1028
        get_local 3
        get_local 1
        i32.or
        i32.store
        get_local 0
        i32.const 8
        i32.add
        set_local 3
        get_local 0
      end
      set_local 1
      get_local 3
      get_local 2
      i32.store
      get_local 1
      get_local 2
      i32.store offset=12
      get_local 2
      get_local 1
      i32.store offset=8
      get_local 2
      get_local 0
      i32.store offset=12
      return
    end
    get_local 3
    i32.const 8
    i32.shr_u
    tee_local 0
    if (result i32)  ;; label = @1
      get_local 3
      i32.const 16777215
      i32.gt_u
      if (result i32)  ;; label = @2
        i32.const 31
      else
        get_local 3
        i32.const 14
        get_local 0
        get_local 0
        i32.const 1048320
        i32.add
        i32.const 16
        i32.shr_u
        i32.const 8
        i32.and
        tee_local 0
        i32.shl
        tee_local 1
        i32.const 520192
        i32.add
        i32.const 16
        i32.shr_u
        i32.const 4
        i32.and
        tee_local 4
        get_local 0
        i32.or
        get_local 1
        get_local 4
        i32.shl
        tee_local 0
        i32.const 245760
        i32.add
        i32.const 16
        i32.shr_u
        i32.const 2
        i32.and
        tee_local 1
        i32.or
        i32.sub
        get_local 0
        get_local 1
        i32.shl
        i32.const 15
        i32.shr_u
        i32.add
        tee_local 0
        i32.const 7
        i32.add
        i32.shr_u
        i32.const 1
        i32.and
        get_local 0
        i32.const 1
        i32.shl
        i32.or
      end
    else
      i32.const 0
    end
    tee_local 1
    i32.const 2
    i32.shl
    i32.const 1332
    i32.add
    set_local 0
    get_local 2
    get_local 1
    i32.store offset=28
    get_local 2
    i32.const 0
    i32.store offset=20
    get_local 2
    i32.const 0
    i32.store offset=16
    block  ;; label = @1
      i32.const 1032
      i32.load
      tee_local 4
      i32.const 1
      get_local 1
      i32.shl
      tee_local 6
      i32.and
      if  ;; label = @2
        block  ;; label = @3
          get_local 0
          i32.load
          tee_local 0
          i32.load offset=4
          i32.const -8
          i32.and
          get_local 3
          i32.eq
          if (result i32)  ;; label = @4
            get_local 0
          else
            i32.const 25
            get_local 1
            i32.const 1
            i32.shr_u
            i32.sub
            set_local 4
            get_local 3
            get_local 1
            i32.const 31
            i32.eq
            if (result i32)  ;; label = @5
              i32.const 0
            else
              get_local 4
            end
            i32.shl
            set_local 4
            loop  ;; label = @5
              get_local 0
              i32.const 16
              i32.add
              get_local 4
              i32.const 31
              i32.shr_u
              i32.const 2
              i32.shl
              i32.add
              tee_local 6
              i32.load
              tee_local 1
              if  ;; label = @6
                get_local 4
                i32.const 1
                i32.shl
                set_local 4
                get_local 1
                i32.load offset=4
                i32.const -8
                i32.and
                get_local 3
                i32.eq
                br_if 3 (;@3;)
                get_local 1
                set_local 0
                br 1 (;@5;)
              end
            end
            get_local 6
            get_local 2
            i32.store
            get_local 2
            get_local 0
            i32.store offset=24
            get_local 2
            get_local 2
            i32.store offset=12
            get_local 2
            get_local 2
            i32.store offset=8
            br 3 (;@1;)
          end
          set_local 1
        end
        get_local 1
        i32.const 8
        i32.add
        tee_local 0
        i32.load
        tee_local 3
        get_local 2
        i32.store offset=12
        get_local 0
        get_local 2
        i32.store
        get_local 2
        get_local 3
        i32.store offset=8
        get_local 2
        get_local 1
        i32.store offset=12
        get_local 2
        i32.const 0
        i32.store offset=24
      else
        i32.const 1032
        get_local 4
        get_local 6
        i32.or
        i32.store
        get_local 0
        get_local 2
        i32.store
        get_local 2
        get_local 0
        i32.store offset=24
        get_local 2
        get_local 2
        i32.store offset=12
        get_local 2
        get_local 2
        i32.store offset=8
      end
    end
    i32.const 1060
    i32.const 1060
    i32.load
    i32.const -1
    i32.add
    tee_local 0
    i32.store
    get_local 0
    if  ;; label = @1
      return
    end
    i32.const 1484
    set_local 0
    loop  ;; label = @1
      get_local 0
      i32.load
      tee_local 2
      i32.const 8
      i32.add
      set_local 0
      get_local 2
      br_if 0 (;@1;)
    end
    i32.const 1060
    i32.const -1
    i32.store)
  (global (;3;) (mut i32) (get_global 1))
  (global (;4;) (mut i32) (get_global 2))
  (export "___errno_location" (func 11))
  (export "_colorFromHexString" (func 7))
  (export "_free" (func 12))
  (export "_malloc" (func 6))
  (elem (get_global 0) 9 8)
  (data (i32.const 1024) "\01"))
