// This function gets your whole document as its `body` and formats
// it as a simple fiction book.
#let book(
  // The book's title.
  title: [Book title],

  // The book's author.
  author: "Author",

  // The paper size to use.
  paper: "iso-b5",

  bibliography-file: none,

  // The book's content.
  body,
) = {
  // Set the document's metadata.
  set document(title: title, author: author)

  // Configure the font.
  set text(font: ("Times New Roman", "NSimSun"))
  show strong: set text(font: ("Times New Roman", "SimHei"))
  show emph: set text(font: ("Times New Roman", "KaiTi"))

  // Configure the page properties.
  set page(
    paper: paper,
    margin: (bottom: 2.25cm, top: 2.25cm),
  )

  // The first page.
  page(align(center + horizon)[
    #text(2em)[*#title*]
    #v(2em, weak: true)
    #text(1.6em, author)
  ])
  // Configure paragraph properties.
  set par(leading: 0.78em, first-line-indent: 2em, justify: true)
  show par: set block(spacing: 1em)

  // Start with a chapter outline.
  outline(title: [本书目录], depth: 2, indent: 1em)
  let chapter_counter = counter(heading.where(level: 1))
  let table_counter = counter(figure.where(kind: table))
  let figure_counter = counter(figure.where(kind: image))

  // Configure page properties.
  set page(
    numbering: "1",

    // The header always contains the book title on odd pages and
    // the chapter title on even pages, unless the page is one
    // the starts a chapter (the chapter title is obvious then).
    header: locate(loc => {
      // Are we on an odd page?
      let i = counter(page).at(loc).first()
      // if calc.odd(i) {
      //   // text(0.95em, smallcaps(title), align: "right")
      //   align(right, text(0.95em, smallcaps(title)))
      // }

      // Are we on a page that starts a chapter? (We also check
      // the previous page because some headings contain pagebreaks.)
      let all = query(heading, loc).filter(it => it.level == 1)
      if all.any(it => it.location().page() == i) {
        return
      }

      // Find the heading of the section we are currently in.
      let before = query(selector(heading).before(loc), loc).filter(it => it.level == 1)

      if before != () {
        let header = before.last()
        if calc.odd(i) {
          // text(0.95em, smallcaps(header.body), align: "right")
          align(right, text(0.95em, smallcaps(header.body)))
        } else {
          // text(0.95em, smallcaps(header.body))
          align(left, text(0.95em, smallcaps(header.body)))
        }
      }
    }),
  )

  set heading(numbering: "1.1.1.")
  show heading: it => locate(loc => {
    let levels = counter(heading).at(loc)

    set text(10pt, weight: 400)
    if it.level == 1 [
      #pagebreak()
      #set align(center)
      #set text(2em, weight: 700)
      #v(20pt, weak: true)
      
      #if it.numbering != none [
        *第*
        #numbering("1", levels.at(0))
        *章*
      ]
      #h(7pt, weak: true)
      *#it.body*
      #v(18pt, weak: true)
      #figure_counter.update(())
      #table_counter.update(())
    ] else if it.level == 2 [
      #set align(center)
      #set text(1.5em, weight: 700)
      #v(16pt, weak: true)
      *第*
      #if it.numbering != none {
        numbering("1.1", levels.at(0), levels.at(1))
      }
      *节*
      #h(7pt, weak: true)
      *#it.body*
      #v(16pt, weak: true)
    ] else [
      #set text(1.2em, weight: 700)
      #v(14pt, weak: true)
      #h(-2em)
      *第*
      #if it.numbering != none {
        numbering("1.1.1", levels.at(0), levels.at(1), levels.at(2))
      }
      *节*
      #h(7pt, weak: true)
      *#it.body*
      #v(14pt, weak: true)
    ]

  })

  set figure(numbering: "1.")

  // 中文图、表
  show figure: it => [
    #let loc = it.location()
    #if not it.has("kind") {
      it
    } else if it.kind == image [
      #set align(center)
      #it.body
      *图#chapter_counter.at(loc).first().#figure_counter.at(loc).first()*
      #h(7pt, weak: true)
      *#it.caption.body*
      #v(1em, weak: true)
    ] else if it.kind == table [
      #set align(center)
      #it.body
      *表#chapter_counter.at(loc).first().#table_counter.at(loc).first()*
      #h(7pt, weak: true)
      *#it.caption.body*
      #v(1em, weak: true)
    ] else [
      it
    ]
  ]

  // 中文引用
  show ref: it => [
    #if it.element == none {
      set text(green)
      it
    } else {
      let el = it.element
      let el_loc = el.location()
      // 如果没有 kind 属性
      if not el.has("kind") [
        #let levels = counter(heading).at(el_loc)
        // levels = (1, 1, ...)
        #set text(purple)
        #if levels.len() == 1 [
          *第*
          #levels.at(0)
          *章*
        ] else if levels.len() == 2 [
          *第*
          #numbering("1.1", levels.at(0), levels.at(1))
          *节*
        ] else if levels.len() == 3 [
          *第*
          #numbering("1.1.1", levels.at(0), levels.at(1), levels.at(2))
          *节*
        ] else [
          #it
        ]
      ] else if el.kind == heading [
        #set text(purple)
        *第*
        #numbering("1", chapter_counter.at(el_loc).first())
        *章*
        #it
      ] else if el.kind == image [
        // 切换为紫色
        #set text(purple)
        *图#chapter_counter.at(el_loc).first().#figure_counter.at(el_loc).first()*
      ] else if el.kind == table [
        #set text(purple)
        *表#chapter_counter.at(el_loc).first().#table_counter.at(el_loc).first()*
        // #el.numbering
      ] else [
        #it
      ]

    }
  ]

  body

  if bibliography-file != none {
    show bibliography: set text(10pt)
    bibliography(bibliography-file, title: text(12pt)[参考文献], style: "ieee")
  }
}

#let graybox(body) = box(fill: color.rgb(0xf0,0xf0,0xf0), inset: (x: 0.125em), outset: (y: 0.25em))[#body]

#let bluetxt(body) = {
  set text(color.rgb(0x40,0x40,0xff)) 
 [*#body*]
}

#let easy = {
  set text(color.rgb(0x40, 0xc0, 0x40))
  [*简单*]
}

#let medium = {
  // orange color
  set text(color.rgb(0xff, 0x80, 0x00))
  [*中等*]
}

#let hard = {
  // red color
  set text(color.rgb(0xff, 0x00, 0x00))
  [*较难*]
}