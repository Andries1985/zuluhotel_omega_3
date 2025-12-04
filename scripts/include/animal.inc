Function Expand_Stats(me)
  if(GetObjProperty(me, "#Expanded"))
    return;
  endif
  var NPCDescFile:=ReadConfigFile("::npcdesc");
  If (NPCDescFile)
    var template:=NPCDescFile[me.npctemplate];
    var value:=CInt(template.str);
    If (value)
      If (value<=210)
        me.SetStr(value);
        me.strength_mod:=0;
      Else
        me.SetStr(210);
        me.strength_mod:=value-210;
      EndIf
    EndIf
    value:=CInt(template.dex);
    If(value)
      If(value<=210)
        me.SetDex(value);
        me.dexterity_mod:=0;
      Else
        me.SetDex(210);
        me.dexterity_mod:=value-210;
      EndIf
    EndIf
    value:=GetConfigInt(FindConfigElem(NPCDescFile, me.npctemplate), "int");
    If(value)
      If(value<=210)
        me.SetInt(value);
        me.intelligence_mod:=0;
      Else
        me.SetInt(210);
        me.intelligence_mod:=value-210;
      EndIf
    EndIf
    value:=CInt(template.hits);
    If (value)
      me.hp:=value;
    EndIf
    value:=CInt(template.mana);
    If (value)
      me.mana:=value;
    EndIf
    value:=CInt(template.stam);
    If (value)
      me.stamina:=value;
    EndIf
    SetObjProperty(me, "#Expanded", 1);
  Else
    SysLog("Setup: Can't open 'npcdesc.cfg'!");
  EndIf
EndFunction
