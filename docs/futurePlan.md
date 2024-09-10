# Future plan

## Overview

- [X] [UNDO](#undo)
- [ ] [multi-languaging](#multi-languaging)
- [ ] [web avalible](#web-avalible)
- [ ] [bundle creation for mac](#bundle-creation-for-mac)
- [ ] [multiple project in same time](#multiple-project-in-same-time) (working on its data managing)
- [ ] [spirit libary](#spirit-libary), also with [import library](#import-library)
- [ ] [stored spirits setting](#stored-spirits-setting)
- [ ] [stored textbox setting](#stored-textbox-setting)
- [ ] [import / replacing / deleting images](#import--replacing--deleting-images)
- [ ] [resolution](#resolution)
- [ ] [animation](#animation)
- [ ] [voicing](#voicing)
- [X] [exit page memory](#exit-page-memory)
- [ ] [drag](#drag)
- [ ] [edit windows?](#edit-windows)
- [ ] *or*, [dragging imgui window outside host window](#or-dragging-imgui-window-outside-host-window)
- [X] [textured buttons](#textured-buttons)
- [ ] [better shortcut keys logic](#textured-buttons)
- [X] [shortcut keys for Mac](#shortcut-keys-for-mac)
- [ ] [game package creation](#game-package-creation)
- [X] [stacking ImGui windows](#stacking-imgui-windows)
- [ ] [Individual ImGui window with color and transparency](#individual-imgui-window-with-color-and-transparency)
- [ ] [dragging pages in library](#dragging-pages-in-library)
- [ ] [refactor](#refactor)
- [ ] [better undo logic](#better-undo-logic)
- [X] [two loading methods](#better-undo-logic)
- [ ] [point and click plug-in](#point-and-click-plug-in)
- [ ] [setted background position/size/ratio](#setted-background-positionsizeratio)
- [ ] [setted multiple windows](#setted-multiple-windows)
- [X] [changable menu background picture](#changable-menu-background-picture)
- [ ] [stored value and trigger](#stored-value-and-trigger)
- [ ] [change project name](#change-project-name)
- [ ] [editable seperate text speed](#editable-seperate-text-speed)
- [ ] [possible better spirit logic](#possible-better-spirit-logic)
- [ ] [better data loading logic](#better-data-loading-logic)
- [ ] [stored page setting](#stored-page-setting)
- [ ] [more naming formatting](#more-naming-formatting)
- [X] [check for '/'](#check-for-)
- [ ] [zoom-in and zoom-out](#zoom-in-and-zoom-out)
- [ ] [multiple pages on one page](#multiple-pages-on-one-page)
- [ ] [two ways to implement library pages](#two-ways-to-implement-library-pages)
- [ ] [save version control](#save-version-control)

## ***UNDO***

this is too important of a thing to be missed, bro u know what i mean? not having a **undo** button in an editor is like no Jerusalem in the western culture, purely unacceptable

## multi-languaging

just some extra work i guess. json file maybe?

## web avalible

i belive by using emscripten and wasm we couold run this c++ project on web

in a way im kinda pro on doing this

哈哈！橙光，启动！🍊

## bundle creation for mac

i need to create a bundle for the mac version of this, or at least the environment set up, or ppl might spend forever trying to set the environment

## multiple project in same time

can open multiple project in the same time? (i wonder who would do that though, nobody would, right?)

## spirit libary

a libary that contains all spirits imported so far, with tags or folders that could seperate them maybe?
(wow i tried so hard to not load all texture, but here we are)

## stored spirits setting

since setting the picture to the same position is just annoying, i am hereby thinking that there should be stored data that could just be pulled out as the spirit is pulled out. Maybe remember how the spirit is used last time and show it like that when pulled?

## stored textbox setting

the textbox setting could be save so that it can be pulled out immediately? im imagining something like a list of textbox presettings with nicknames

## import / replacing / deleting images

should i store it in jpg or texture... I would prefer jpg though
oh btw, should i set the image to be at least / at most certain size? can't think of why i should, but that sounds very pro.

## resolution

currently the window can be dragged any size and could resize in whatever, but characters (which i mean like, human?) are in best always in a ratio right? not gonna be fat(

    what about if ratio's different, always show <all_of_shorter_side> * <part_of_longer_side> that kind of size calculation? am i able to let the user choose on showing which part of longer side? complicated

## animation

man Codename Bakery's animation is too good, i need good animations in this game egine too. At least some simple motion and video player right? right??

(btw Codename Bakery's hidden ending is so stupid, Imma call a duel on our lovely bro 羽中，等死吧你！)

## voicing

honestly can't really imagine ppl using this application to makes games that contains voicing, but one need to have dream, so i'm gonna add this feature.

## exit page memory

i think opening the project and the project opens at the last modification is a cool thing. though is this really necessary?(

## drag

i mean, editing is very needing dragging, i think it's a must

## edit windows?

like you know, all other engines like unity or sai2, where you have a window showing the game inside the big host window. though it's not cool

## *or*, dragging imgui window outside host window

this is cool, i like it

## textured buttons

if i want to do setting then i need buttons, if i have buttons there should be icons, if there are clickable icons, ~~then why don't i just make a point-and-click game engine then~~ should i limit its feature or just make them able to do anthing

## better shortcut keys logic

just incase there are people who is using this editor without even having a mouse/touchpad/drawpad... who???

## shortcut keys for Mac

This is one of the many reasons why i don't like mac, like literally, why *command*?

## game package creation

My UI designing skill is so bad that I couldn't think of a good place to let the user set the icon/name/blablabla for their game. Time to grab my art major ~~slave~~ friend for ~~free labor~~ help.

## stacking ImGui windows

Like if i click on it it will be moved to the top. I hope this is a built-in feature, please

## Individual ImGui window with color and transparency

like there would be a slide bar on top or bottom of the ImGui window, so that you can change it's color and transparency all the time, kewl

## dragging pages in library

wow, very kewl, and very hard(. I do have the idea on how to make the data move, but how do i make the animation 👽

## refactor

man, it's really a mess, i have to admit

## better undo logic

saving and reading the whole file is a bit uh, so i'm thinking of saving and reading replacing parts, implementing a logic for undo similar to the logic for texturing. what i'm thinking is like seperating `importing spirit`, `page content`, `game setting` and such with idex numbers, so that undo will contain something like a `pair<int, string>` so that the code know after reading where to go and change. Oh, ***《`pair<int, T>`》***, mind explode
> man this is getting **urgent** as i am stuffing way too many things (some uneccessary) in a project's Data

## two loading methods

extention for [better undo logic](#better-undo-logic). Currently I am thinking of two ways to store the undo data, either to store the corresponding struct directly, using something like a `std::variant`, which would cause more cost on storage. The other way is to store everything with string, and load the string into struct when `undo` is called, which would cost more on runtime speed. What about i just make it a user-choosing thing, and have both function implemented.

## point and click plug-in

reference to [textured buttons](#textured-buttons), i'm having more thoughts on it rn

## setted background position/size/ratio

I canceled this feature in V0.0.2, but now thinking of it isn't it kewl if your screen suddenly jumped away and changed it's size, kinda meta-ish feeling. Let me think... Kinda like "The Pedestrian" without background?

## changable menu background picture

by which i mean during the progress of the story the menu picture will change, very astonishing feature in some games, don't think it's gonna be hard to achieve

## stored value and trigger

haha, totally forgot abt this, how could one not have this though

> : do u think u r a visual novel engine
>
> : yes, i think i am

## setted multiple windows

Some extened thoughts on [setted background position/size/ratio](#setted-background-positionsizeratio), just thinking, but if the game could have multiple windows, setted in rigid positions on the screen, that would be so kewl. I can think of ways to do it, but enabling it with engine would be hard

## change project name

Or else all my project names will be named in something like 新川崎(仮), but I would prefer トゲナシトゲアリ

## editable seperate text speed

like when a person say something empasizing or REALLY, REALLY, SLOW, i think making the speed of text slow down (or speed up) is fun

> 现在年轻女孩一点矜持都没有在那边胡说八道讲得天花乱坠大婶我年轻时代根本不会有这种事每个人都相亲相爱彼此着想可是看看现在有事没事在背后说人坏话搞得大家人心惶惶的说起来现在年轻人都禁不起念讲个两三句就回你个十几句都不懂得尊重老人不知道是怎么教的大婶我真难过……呜喔喔喔喔喔喔喔喔喔喔喔喔！

## possible better spirit logic

better logic for spirit... somewhat same as the always metioned thing, I don't think saving same spirit for so many times is a good thing... Maybe with the spirit list i could save a pointer in the spirit that points to the object inside the spirit map/list/vector/whatever. But what would be the difference here(

## better data loading logic

currently im loading all pages in the beginning, but that could take too much memory. Should i instead load only the current page? but also a page does not take that much space, does it really worth it?

## stored page setting

kind of similar to what i mentioned in [stored spirits setting](#stored-spirits-setting), where the page is stored as a format and can be pulled out when wanted. Infact ButtonInterface is designed like that, so I'm thinking of applying the same feature onto this too. Not gonna be hard code-wise, the hardest part is UI design(

wait, i don't even need buttonInterface if i implement this.

am i stupid

## more naming formatting

eh, for some reason, my functions are all named in myseriously incoherant ways, like i just figured encryption functions named in "encrypt", "encrypted", "encryptToString", "encryptIntoFile", "exportInString", "save", "toString" and "to_string"... very unique, many diversion in my project, so politically correct(

## check for '/'

I did implement encryption and decryption, but can't remember which step i checked it, so i guess i need to walk through all the encryption and decryption functions to figure this out ;)

> this can be skipped with the bitloading method, maybe I'll left it here now

## zoom-in and zoom-out

honestly can't think of a reason anyone's doing it, but i guess, y not

## multiple pages on one page

this is my alternative solution for buttonInterface and extention for [stored page setting](#stored-page-setting). I am thinking that I can just replace buttonInterface with page, and place it on top of another page for the same result. I was thinking earlier that one can choose to change the drawing order for every page so one formatted page could store both background and buttons, but mean while this would cause more works on `_draw_order`, probably with a extra value like `_changed_draw_order`, which is kinda annoying. I would make the formatted page all glued together for now (so one is unable to insert anything in between elements inside a formatted page).

Another thing is that I haven't decide on the number of interfaces one page could have. Multiple would be great, but implementing this feature could be confusing to my brain.

oh wait i figured it out (in about 2 minutes), nvm

## two ways to implement library pages

continuing on the topic, while implementing the feature maybe i can seperate it in two way, 1 is to include the page as a "format page", which like i just mentioned will be glued. The second way is to include it as a "template", which will not create a pointer but copy the content of that page into this page. And one can change from "format page" to "template page" anytime (not vise versa though).

## import library

with the current data structure, i would say the only way to access library saved from other projects is to import them. Like letting the user to choose the project containing the library they want to load. But this would be challenging, cuz i need to copy all the image to this folder too.

## save version control

since the binary save data changes, there should be this version control thing that makes sure which version of save it is reding. well, this could be done after release.
