const imageList = [
  '../res/img/911_1.jpg',
  '../res/img/911_2.jpg',
  '../res/img/911_3.jpg',
  '../res/img/911_4.jpg',
  '../res/img/911_5.jpg',
  '../res/img/911_6.jpg',
  '../res/img/911_7.jpg',
  '../res/img/911_8.jpg',
  '../res/img/911_9.jpg',
  '../res/img/911_10.jpg',
  '../res/img/911_11.jpg',
  '../res/img/911_12.jpg',
  '../res/img/911_13.jpg',
  '../res/img/911_14.jpg',
  '../res/img/911_15.jpg',
  '../res/img/911_16.jpg',

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