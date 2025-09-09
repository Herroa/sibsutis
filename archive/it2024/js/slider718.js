const imageList = [
  '../res/img/718_1.png',
  '../res/img/718_2.jpg',
  '../res/img/718_3.jpg',
  '../res/img/718_4.jpg',
  '../res/img/718_5.jpg',
  '../res/img/718_6.jpg',
  '../res/img/718_7.jpg'
]
const img = document.querySelector('.img')
const next = document.getElementById('next')
const previous = document.getElementById('previous')
var numImage = 0
img.src = imageList[numImage]

next.addEventListener('click', () => {
  numImage++
  if (numImage >= imageList.length) {
    numImage = 0
  }
  console.log(numImage >= imageList.length)
  console.log(numImage)
  img.src = imageList[numImage]
})
previous.addEventListener('click', () => {
  numImage--
  if (numImage < 0) {
    numImage = imageList.length - 1
  }
  console.log(numImage >= imageList.length)
  console.log(numImage)
  img.src = imageList[numImage]
})